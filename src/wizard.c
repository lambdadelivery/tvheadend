/*
 *  tvheadend, Wizard
 *  Copyright (C) 2015 Jaroslav Kysela
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tvheadend.h"
#include "access.h"
#include "settings.h"
#include "wizard.h"

/*
 *
 */

static const void *empty_get(void *o)
{
  prop_sbuf[0] = '\0';
  return &prop_sbuf_ptr;
}

static const void *icon_get(void *o)
{
  strcpy(prop_sbuf, "docresources/tvheadendlogo.png");
  return &prop_sbuf_ptr;
}

#define SPECIAL_PROP(idval, getfcn) { \
  .type = PT_STR, \
  .id   = idval, \
  .name = "", \
  .get  = getfcn, \
  .opts = PO_RDONLY | PO_NOUI \
}

#define PREV_BUTTON(page) SPECIAL_PROP("page_prev_" STRINGIFY(page), empty_get)
#define NEXT_BUTTON(page) SPECIAL_PROP("page_next_" STRINGIFY(page), empty_get)
#define LAST_BUTTON()     SPECIAL_PROP("page_last", empty_get)
#define ICON()            SPECIAL_PROP("icon", icon_get)
#define DESCRIPTION(page) SPECIAL_PROP("description", wizard_description_##page)

#define DESCRIPTION_FCN(page, desc) \
static const void *wizard_description_##page(void *o) \
{ \
  static const char *t = desc; \
  return &t; \
}

/*
 *
 */

static void page_free(wizard_page_t *page)
{
  free((char *)page->idnode.in_class);
  free(page);
}

static wizard_page_t *page_init(const char *class_name, const char *caption)
{
  wizard_page_t *page = calloc(1, sizeof(*page));
  idclass_t *ic = calloc(1, sizeof(*ic));
  page->idnode.in_class = ic;
  ic->ic_caption = caption;
  ic->ic_class = ic->ic_event = class_name;
  ic->ic_perm_def = ACCESS_ADMIN;
  page->free = page_free;
  return page;
}

/*
 * Hello
 */

static const void *hello_get_network(void *o)
{
  strcpy(prop_sbuf, "Test123");
  return &prop_sbuf_ptr;
}

static int hello_set_network(void *o, const void *v)
{
  return 0;
}

DESCRIPTION_FCN(hello, N_("\
Enter allowed network (like 192.168.1.0/24) and user logins for administrator and ordinary user. \
If the username is empty, anonymous access will be allowed.\
"))

wizard_page_t *wizard_hello(void)
{
  static const property_group_t groups[] = {
    {
      .name     = N_("Network access"),
      .number   = 1,
    },
    {
      .name     = N_("Administrator login"),
      .number   = 2,
    },
    {
      .name     = N_("User login"),
      .number   = 3,
    },
    {}
  };
  static const property_t props[] = {
    {
      .type     = PT_STR,
      .id       = "network",
      .name     = N_("Allowed network"),
      .get      = hello_get_network,
      .set      = hello_set_network,
      .group    = 1
    },
    {
      .type     = PT_STR,
      .id       = "admin_username",
      .name     = N_("Admin username"),
      .get      = hello_get_network,
      .set      = hello_set_network,
      .group    = 2
    },
    {
      .type     = PT_STR,
      .id       = "admin_password",
      .name     = N_("Admin password"),
      .get      = hello_get_network,
      .set      = hello_set_network,
      .group    = 2
    },
    {
      .type     = PT_STR,
      .id       = "username",
      .name     = N_("Username"),
      .get      = hello_get_network,
      .set      = hello_set_network,
      .group    = 3
    },
    {
      .type     = PT_STR,
      .id       = "password",
      .name     = N_("Password"),
      .get      = hello_get_network,
      .set      = hello_set_network,
      .group    = 3
    },
    ICON(),
    DESCRIPTION(hello),
    NEXT_BUTTON(network),
    {}
  };
  wizard_page_t *page =
    page_init("wizard_hello",
    N_("Welcome - Tvheadend - your TV streaming server and video recorder"));
  idclass_t *ic = (idclass_t *)page->idnode.in_class;
  ic->ic_properties = props;
  ic->ic_groups = groups;
  return page;
}

/*
 * Network settings
 */

DESCRIPTION_FCN(network, N_("\
Create networks.\
"))


wizard_page_t *wizard_network(void)
{
  static const property_t props[] = {
    {
      .type     = PT_STR,
      .id       = "network1",
      .name     = N_("Network 1"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    {
      .type     = PT_STR,
      .id       = "network2",
      .name     = N_("Network 2"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    {
      .type     = PT_STR,
      .id       = "network3",
      .name     = N_("Network 3"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    ICON(),
    DESCRIPTION(network),
    PREV_BUTTON(hello),
    NEXT_BUTTON(input),
    {}
  };
  wizard_page_t *page = page_init("wizard_network", N_("Network settings"));
  idclass_t *ic = (idclass_t *)page->idnode.in_class;
  ic->ic_properties = props;
  return page;
}

/*
 * Input settings
 */

DESCRIPTION_FCN(input, N_("\
Assign inputs to networks.\
"))


wizard_page_t *wizard_input(void)
{
  static const property_t props[] = {
    {
      .type     = PT_STR,
      .id       = "input1",
      .name     = N_("Input 1 network"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    {
      .type     = PT_STR,
      .id       = "input2",
      .name     = N_("Input 2 network"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    {
      .type     = PT_STR,
      .id       = "input3",
      .name     = N_("Input 3 network"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    ICON(),
    DESCRIPTION(input),
    PREV_BUTTON(network),
    NEXT_BUTTON(status),
    {}
  };
  wizard_page_t *page = page_init("wizard_input", N_("Input / tuner settings"));
  idclass_t *ic = (idclass_t *)page->idnode.in_class;
  ic->ic_properties = props;
  return page;
}

/*
 * Status
 */

DESCRIPTION_FCN(status, N_("\
Show the scan status.\
"))


wizard_page_t *wizard_status(void)
{
  static const property_t props[] = {
    {
      .type     = PT_STR,
      .id       = "muxes",
      .name     = N_("Found muxes"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    {
      .type     = PT_STR,
      .id       = "services",
      .name     = N_("Found services"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    ICON(),
    DESCRIPTION(status),
    PREV_BUTTON(input),
    NEXT_BUTTON(mapping),
    {}
  };
  wizard_page_t *page = page_init("wizard_status", N_("Scan status"));
  idclass_t *ic = (idclass_t *)page->idnode.in_class;
  ic->ic_properties = props;
  return page;
}

/*
 * Service Mapping
 */

DESCRIPTION_FCN(mapping, N_("\
Do the service mapping to channels.\
"))


wizard_page_t *wizard_mapping(void)
{
  static const property_t props[] = {
    {
      .type     = PT_STR,
      .id       = "pnetwork",
      .name     = N_("Select network"),
      .get      = hello_get_network,
      .set      = hello_set_network,
    },
    ICON(),
    DESCRIPTION(mapping),
    PREV_BUTTON(status),
    LAST_BUTTON(),
    {}
  };
  wizard_page_t *page = page_init("wizard_service_map", N_("Service mapping"));
  idclass_t *ic = (idclass_t *)page->idnode.in_class;
  ic->ic_properties = props;
  return page;
}
