//! @file directory_iteration.c
//!
//!
//! @brief It is application for all recursive iteration of directories and files in directory.
//! 
//!
//! @ Author: Usman Ashraf 
//! @ email:  u.ashraf1988@gmail.com
//!
//! GNU Lesser General Public License v3.0, free to use, re-use, distriution and modification
//! provided this above header information. Author take no responsibility for any outcome of
//! this code. 
//!
//! If you need more information, please, contact me at my above email address.
//
//! Successfully executed under OS:
//!                                RedHat 7
//!                                RedHat 5
//!                                CentOS 7
//!                                Ubuntu 16
//!                                yocto-created minimal image
//!                                Windows 10
//!
//!
//! Compiled with GCC Version > 4.5


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <string.h>
#include <errno.h> 

#include <dirent.h>
#include <limits.h>
#include <sys/stat.h> /* structure stat and stat(const char*, struct stat *) api */

static void
list_dir (const char * dir_name)
{
    DIR * pDirectory;
    struct stat st_buf;
    if (-1==stat(dir_name, &st_buf)){
	fprintf(stderr, "Stat Error: %s: %s", dir_name, strerror(errno));
	exit (EXIT_FAILURE);
    }

    /* Open the directory specified by "dir_name". */
    pDirectory = opendir (dir_name);
    if (! pDirectory) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }

    while (1) {
        struct dirent * pDirEntry;
        const char * d_name;
	int isDir=0;
        struct stat st_buf1;

        pDirEntry = readdir (pDirectory);
        if (! pDirEntry) {
            /* There are no more entries. */
            break;
        }

        d_name = pDirEntry->d_name;
        if (-1==stat(d_name, &st_buf1)){
	   fprintf(stderr, "Stat Error: %s: %s", dir_name, strerror(errno));
	   exit (EXIT_FAILURE);
        }

	if ( (st_buf1.st_mode & S_IFMT) == S_IFDIR){ /* S_IFMT, S_IFDIR, S_IFCHR, S_IFBLK, S_IFREG */
           isDir = 1;
	}

        if ( isDir ) {
	    printf ("Directory: %s/%s\n", dir_name, d_name);
	} else {
	    printf ("File: %s/%s\n", dir_name, d_name);
	}

        if ( isDir ) {
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                list_dir (path);
            }
	}
    }

    /* After going through all the entries, close the directory. */
    if (closedir (pDirectory)) {
        fprintf (stderr, "Could not close '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
}


int main ()
{
    list_dir (".");
    return 0;
}
