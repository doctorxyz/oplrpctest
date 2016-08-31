#include "irx.h"
#include "types.h"
#include "stdio.h"
#include "thbase.h"
#include "sifman.h"
#include "sifcmd.h"
#include "sysmem.h"


#define MODNAME			"freemem"

#define M_PRINTF(format, args...)	\
	printf(MODNAME ": " format, ## args)

#define M_ERROR(format, args...) M_PRINTF("ERROR: " format, ## args)

#if 1
#define M_DEBUG M_PRINTF
#else
#define M_DEBUG(format, args...)
#endif

#define FREEMEM_IRX		0xFEEFAAF0	/*random hopefully unique number for the interface */
/* function(s) to be called */
#define FM_CMD_MEMSIZE		1
#define FM_CMD_MAXFREEMEM	2
#define FM_CMD_TOTFREEMEM	3

IRX_ID(MODNAME, 1, 0);

static SifRpcDataQueue_t rpc_queue __attribute__((aligned(64)));
static SifRpcServerData_t rpc_server __attribute((aligned(64)));
static u8 _rpc_buffer[1024] __attribute((aligned(64)));


static void *
_rpc_cmd_handler(u32 command, void *buffer, int size)
{
	int ret;

	switch (command) {
		case FM_CMD_MEMSIZE:
			M_DEBUG("FM_CMD_MEMSIZE\n");
			ret = QueryMemSize();
			break;
		case FM_CMD_MAXFREEMEM:
			M_DEBUG("FM_CMD_MAXFREEMEM\n");
			ret = QueryMaxFreeMemSize();
			break;
		case FM_CMD_TOTFREEMEM:
			M_DEBUG("FM_CMD_TOTFREEMEM\n");
			ret = QueryTotalFreeMemSize();
			break;

		default:
			M_ERROR("unknown cmd %lu\n", command);
			ret = -1;
			break;
	}

	((u32 *)buffer)[0] = ret;

	return buffer;
}

static void
_rpc_thread(void* param)
{
	int tid;

	M_PRINTF("RPC thread running\n");

	tid = GetThreadId();

	sceSifSetRpcQueue(&rpc_queue, tid);
	sceSifRegisterRpc(&rpc_server, FREEMEM_IRX, (void *)_rpc_cmd_handler, _rpc_buffer, 0, 0, &rpc_queue);
	sceSifRpcLoop(&rpc_queue);
}

int _start(int argc, char *argv[])
{
	iop_thread_t param;
	int th;

	/*create thread*/
	param.attr	= TH_C;
	param.thread	= _rpc_thread;
	param.priority	= 40;
	param.stacksize	= 0x1000;
	param.option	= 0;

	th = CreateThread(&param);
	if (th > 0) {
		StartThread(th, 0);
		return 0;
	} else {
		M_ERROR("unable to create thread (%d)\n", th);
		return 1;
	}

	M_PRINTF("running\n");

	return 0;
}
