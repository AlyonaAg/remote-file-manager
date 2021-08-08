

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Tue Oct 27 21:30:43 2020
 */
/* Compiler settings for interface_BSIT.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, app_config, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __interface_BSIT_h__
#define __interface_BSIT_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __interface_BSIT_INTERFACE_DEFINED__
#define __interface_BSIT_INTERFACE_DEFINED__

/* interface interface_BSIT */
/* [implicit_handle][version][uuid] */ 

void impersonation( 
    /* [in] */ handle_t IDL_handle,
    /* [string][in] */ unsigned char *login,
    /* [string][in] */ unsigned char *password,
    /* [out] */ unsigned char *result);

void send_( 
    /* [string][in] */ unsigned char *path,
    /* [in] */ unsigned char data[ 25900000 ],
    /* [in] */ unsigned int length,
    /* [out] */ unsigned char *result);

void download_( 
    /* [string][in] */ unsigned char *path,
    /* [out] */ unsigned char data[ 25900000 ],
    /* [out] */ unsigned int *length,
    /* [out] */ unsigned char *result);

void delete_( 
    /* [string][in] */ unsigned char *path,
    /* [out] */ unsigned char *result);

void end_( 
    /* [out] */ unsigned char *result);


extern handle_t IDL_handle;


extern RPC_IF_HANDLE interface_BSIT_v1_0_c_ifspec;
extern RPC_IF_HANDLE interface_BSIT_v1_0_s_ifspec;
#endif /* __interface_BSIT_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


