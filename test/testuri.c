/* Copyright 2000-2004 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>

#include "testutil.h"
#include "apr_general.h"
#include "apr_uri.h"

struct aup_test {
    const char *uri;
    apr_status_t rv;
    const char *scheme;
    const char *hostinfo;
    const char *user;
    const char *password;
    const char *hostname;
    const char *port_str;
    const char *path;
    const char *query;
    const char *fragment;
    apr_port_t  port;
};

struct aup_test aup_tests[] =
{
    {
        "http://127.0.0.1:9999/asdf.html",
        0, "http", "127.0.0.1:9999", NULL, NULL, "127.0.0.1", "9999", "/asdf.html", NULL, NULL, 9999
    },
    {
        "http://127.0.0.1:9999a/asdf.html",
        APR_EGENERAL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0
    },
    {
        "http://[::127.0.0.1]:9999/asdf.html",
        0, "http", "[::127.0.0.1]:9999", NULL, NULL, "::127.0.0.1", "9999", "/asdf.html", NULL, NULL, 9999
    },
    {
        "http://[::127.0.0.1]:9999a/asdf.html",
        APR_EGENERAL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0
    },
    {
        "/error/include/top.html",
        0, NULL, NULL, NULL, NULL, NULL, NULL, "/error/include/top.html", NULL, NULL, 0
    },
    {
        "/error/include/../contact.html.var",
        0, NULL, NULL, NULL, NULL, NULL, NULL, "/error/include/../contact.html.var", NULL, NULL, 0
    },
    {
        "/",
        0, NULL, NULL, NULL, NULL, NULL, NULL, "/", NULL, NULL, 0
    },
    {
        "/manual/",
        0, NULL, NULL, NULL, NULL, NULL, NULL, "/manual/", NULL, NULL, 0
    },
    {
        "/cocoon/developing/graphics/Using%20Databases-label_over.jpg",
        0, NULL, NULL, NULL, NULL, NULL, NULL, "/cocoon/developing/graphics/Using%20Databases-label_over.jpg", NULL, NULL, 0
    },
    {
        "http://sonyamt:garbage@127.0.0.1/filespace/",
        0, "http", "sonyamt:garbage@127.0.0.1", "sonyamt", "garbage", "127.0.0.1", NULL, "/filespace/", NULL, NULL, 0
    },
    {
        "http://sonyamt:garbage@[fe80::1]/filespace/",
        0, "http", "sonyamt:garbage@[fe80::1]", "sonyamt", "garbage", "fe80::1", NULL, "/filespace/", NULL, NULL, 0
    },
    {
        "http://sonyamt@[fe80::1]/filespace/?arg1=store",
        0, "http", "sonyamt@[fe80::1]", "sonyamt", NULL, "fe80::1", NULL, "/filespace/", "arg1=store", NULL, 0
    },
    {
        "//www.apache.org/",
        0, NULL, "www.apache.org", NULL, NULL, "www.apache.org", NULL, "/", NULL, NULL, 0
    },
};

struct uph_test {
    const char *hostinfo;
    apr_status_t rv;
    const char *hostname;
    const char *port_str;
    apr_port_t port;
};

struct uph_test uph_tests[] =
{
    {
        "www.ibm.com:443",
        0, "www.ibm.com", "443", 443
    },
    {
        "[fe80::1]:443",
        0, "fe80::1", "443", 443
    },
    {
        "127.0.0.1:443",
        0, "127.0.0.1", "443", 443
    },
    {
        "127.0.0.1",
        APR_EGENERAL, NULL, NULL, 0
    },
    {
        "[fe80:80",
        APR_EGENERAL, NULL, NULL, 0
    },
    {
        "fe80::80]:443",
        APR_EGENERAL, NULL, NULL, 0
    }
};

#if 0
static void show_info(apr_status_t rv, apr_status_t expected, const apr_uri_t *info)
{
    if (rv != expected) {
        fprintf(stderr, "  actual rv: %d    expected rv:  %d\n", rv, expected);
    }
    else {
        fprintf(stderr, 
                "  scheme:           %s\n"
                "  hostinfo:         %s\n"
                "  user:             %s\n"
                "  password:         %s\n"
                "  hostname:         %s\n"
                "  port_str:         %s\n"
                "  path:             %s\n"
                "  query:            %s\n"
                "  fragment:         %s\n"
                "  hostent:          %p\n"
                "  port:             %u\n"
                "  is_initialized:   %u\n"
                "  dns_looked_up:    %u\n"
                "  dns_resolved:     %u\n",
                info->scheme, info->hostinfo, info->user, info->password,
                info->hostname, info->port_str, info->path, info->query,
                info->fragment, info->hostent, info->port, info->is_initialized,
                info->dns_looked_up, info->dns_resolved);
    }
}
#endif

static void test_aup(abts_case *tc, void *data)
{
    int i;
    apr_status_t rv;
    apr_uri_t info;
    struct aup_test *t;
    const char *s = NULL;

    for (i = 0; i < sizeof(aup_tests) / sizeof(aup_tests[0]); i++) {
        memset(&info, 0, sizeof(info));
        t = &aup_tests[i];
        rv = apr_uri_parse(p, t->uri, &info);
        ABTS_INT_EQUAL(tc, rv, t->rv);
        if (t->rv == APR_SUCCESS) {
            ABTS_STR_EQUAL(tc, info.scheme, t->scheme);
            ABTS_STR_EQUAL(tc, info.hostinfo, t->hostinfo);
            ABTS_STR_EQUAL(tc, info.user, t->user);
            ABTS_STR_EQUAL(tc, info.password, t->password);
            ABTS_STR_EQUAL(tc, info.hostname, t->hostname);
            ABTS_STR_EQUAL(tc, info.port_str, t->port_str);
            ABTS_STR_EQUAL(tc, info.path, t->path);
            ABTS_STR_EQUAL(tc, info.query, t->query);
            ABTS_STR_EQUAL(tc, info.user, t->user);
            ABTS_INT_EQUAL(tc, info.port, t->port);

            s = apr_uri_unparse(p, &info, APR_URI_UNP_REVEALPASSWORD);
            ABTS_STR_EQUAL(tc, s, t->uri);
        }
    }
}

static void test_uph(abts_case *tc, void *data)
{
    int i;
    apr_status_t rv;
    apr_uri_t info;
    struct uph_test *t;

    for (i = 0; i < sizeof(uph_tests) / sizeof(uph_tests[0]); i++) {
        memset(&info, 0, sizeof(info));
        t = &uph_tests[i];
        rv = apr_uri_parse_hostinfo(p, t->hostinfo, &info);
        ABTS_INT_EQUAL(tc, rv, t->rv);
        if (t->rv == APR_SUCCESS) {
            ABTS_STR_EQUAL(tc, info.hostname, t->hostname);
            ABTS_STR_EQUAL(tc, info.port_str, t->port_str);
            ABTS_INT_EQUAL(tc, info.port, t->port);
        }
    }
}

abts_suite *testuri(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, test_aup, NULL);
    abts_run_test(suite, test_uph, NULL);

    return suite;
}

