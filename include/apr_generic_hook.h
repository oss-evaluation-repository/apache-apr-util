/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000-2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#ifndef APR_GENERIC_HOOK_H
#define APR_GENERIC_HOOK_H

#include "apr_tables.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @package Apache hooks functions
 */

#define APR_DECLARE_GENERIC_HOOK(ns,ret,name,args) \
typedef ret ns##_HOOK_##name args;

APU_DECLARE(void) apr_hook_generic_add(const char *szName,void (*pfn)(void),
				   const char * const *aszPre,
				   const char * const *aszSucc,int nOrder);

/**
 * Hook to a generic hook.
 *
 * @param name The name of the hook
 * @param pfn A pointer to a function that will be called
 * @param aszPre a NULL-terminated array of strings that name modules whose hooks should precede this one
 * @param aszSucc a NULL-terminated array of strings that name modules whose hooks should succeed this one
 * @param nOrder an integer determining order before honouring aszPre and aszSucc (for example HOOK_MIDDLE)
 */

#define APR_HOOK_GENERIC(name,pfn,aszPre,aszSucc,nOrder) \
    ((void (*)(const char *,HOOK_##name *,const char * const *, \
	       const char * const *,int))&apr_hook_generic_add)(#name,pfn,aszPre, \
							   aszSucc, nOrder)

APU_DECLARE(apr_array_header_t *) apr_hook_generic_get(const char *szName);

/**
 * Implement a generic hook that runs until one of the functions
 * returns something other than OK or DECLINE.
 *
 * @param ret The type of the return value of the hook
 * @param name The name of the hook
 * @param args_decl The declaration of the arguments for the hook
 * @param args_used The names for the arguments for the hook
 * @deffunc int APR_IMPLEMENT_EXTERNAL_HOOK_RUN_ALL(link, name, args_decl, args_use)
 */
#define APR_IMPLEMENT_GENERIC_HOOK_RUN_ALL(ns,link,ret,name,args_decl,args_use,ok,decline) \
link##_DECLARE(ret) ns##_run_##name args_decl \
    { \
    ns##_LINK_##name *pHook; \
    int n; \
    ret rv; \
    apr_array_header_t *pHookArray=apr_hook_generic_get(#name); \
\
    if(!pHookArray) \
	return ok; \
\
    pHook=(ns##_LINK_##name *)pHookArray->elts; \
    for(n=0 ; n < pHookArray->nelts ; ++n) \
	{ \
	rv=pHook[n].pFunc args_use; \
\
	if(rv != ok && rv != decline) \
	    return rv; \
	} \
    return ok; \
    }

#ifdef __cplusplus
}
#endif

#endif /* APR_GENERIC_HOOK_H */
