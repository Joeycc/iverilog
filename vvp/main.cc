/*
 * Copyright (c) 2001 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#if !defined(WINNT)
#ident "$Id: main.cc,v 1.11 2001/05/11 02:06:14 steve Exp $"
#endif

# include  "config.h"
# include  "debug.h"
# include  "parse_misc.h"
# include  "compile.h"
# include  "schedule.h"
# include  "vpi_priv.h"
# include  <stdio.h>
#if defined(HAVE_GETOPT_H)
# include  <getopt.h>
#endif


const char*module_path = MODULE_DIR;
unsigned module_cnt = 0;
const char*module_tab[64];

extern void vpi_mcd_init(void);

int main(int argc, char*argv[])
{
      int opt;
      unsigned flag_errors = 0;
      const char*design_path = 0;
      bool debug_flag = false;

      while ((opt = getopt(argc, argv, "dM:m:")) != EOF) switch (opt) {
	  case 'd':
	    debug_flag = true;
	    break;
	  case 'M':
	    module_path = optarg;
	    break;
	  case 'm':
	    module_tab[module_cnt++] = optarg;
	    break;
	  default:
	    flag_errors += 1;
      }

      if (flag_errors)
	    return flag_errors;

      if (optind == argc) {
	    fprintf(stderr, "%s: no input file.\n", argv[0]);
	    return -1;
      }

      design_path = argv[optind];

	/* This is needed to get the MCD I/O routines ready for
	   anything. It is done early because it is plausible that the
	   compile might affect it, and it is cheap to do. */

      vpi_mcd_init();

      compile_init();
#if 0
      for (unsigned idx = 0 ;  idx < module_cnt ;  idx += 1)
	    vpip_load_module(module_tab[idx], module_path);
#endif
      if (int rc = compile_design(design_path))
	    return rc;
      compile_cleanup();

      if (compile_errors > 0) {
	    fprintf(stderr, "%s: Program not runnable, %u errors.\n",
		    design_path, compile_errors);
	    return compile_errors;
      }

      if (debug_flag)
	    breakpoint();

      schedule_simulate();

      return 0;
}

/*
 * $Log: main.cc,v $
 * Revision 1.11  2001/05/11 02:06:14  steve
 *  Add the --enable-vvp-debug option to the configure
 *  script of vvp, and detect getopt.h.
 *
 * Revision 1.10  2001/05/09 04:23:19  steve
 *  Now that the interactive debugger exists,
 *  there is no use for the output dump.
 *
 * Revision 1.9  2001/05/08 23:32:26  steve
 *  Add to the debugger the ability to view and
 *  break on functors.
 *
 *  Add strengths to functors at compile time,
 *  and Make functors pass their strengths as they
 *  propagate their output.
 *
 * Revision 1.8  2001/04/04 04:33:08  steve
 *  Take vector form as parameters to vpi_call.
 *
 * Revision 1.7  2001/03/23 02:40:22  steve
 *  Add the :module header statement.
 *
 * Revision 1.6  2001/03/22 22:38:14  steve
 *  Detect undefined system tasks at compile time.
 *
 * Revision 1.5  2001/03/22 21:26:54  steve
 *  Compile in a default VPI module dir.
 *
 * Revision 1.4  2001/03/20 06:16:24  steve
 *  Add support for variable vectors.
 *
 * Revision 1.3  2001/03/18 04:35:18  steve
 *  Add support for string constants to VPI.
 *
 * Revision 1.2  2001/03/16 01:44:34  steve
 *  Add structures for VPI support, and all the %vpi_call
 *  instruction. Get linking of VPI modules to work.
 *
 * Revision 1.1  2001/03/11 00:29:38  steve
 *  Add the vvp engine to cvs.
 *
 */

