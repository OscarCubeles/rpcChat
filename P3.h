/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _P3_H_RPCGEN
#define _P3_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


#define RPC 0x20000001
#define FIRST_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define write_chat 0
extern  void * write_chat_1(char **, CLIENT *);
extern  void * write_chat_1_svc(char **, struct svc_req *);
#define get_chat 1
extern  char ** get_chat_1(int *, CLIENT *);
extern  char ** get_chat_1_svc(int *, struct svc_req *);
extern int rpc_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define write_chat 0
extern  void * write_chat_1();
extern  void * write_chat_1_svc();
#define get_chat 1
extern  char ** get_chat_1();
extern  char ** get_chat_1_svc();
extern int rpc_1_freeresult ();
#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_P3_H_RPCGEN */
