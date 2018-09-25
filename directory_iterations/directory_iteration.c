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
//!
//!
//! Compiled with GCC Version > 4.5


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> /* dev_t, ino_t types used in stat */
#include <string.h>
#include <errno.h> /* errno, strerror */

/* "readdir" and DIR and dirent structure etc. are defined here. */
#include <dirent.h>

/* limits.h defines "PATH_MAX". */
#include <limits.h>

#include <sys/stat.h> /* structure stat and stat(const char*, struct stat *) api */

/* List the files in "dir_name". */

static void
list_dir (const char * dir_name)
{

    struct stat st_buf;
    if (-1==stat(dir_name, &st_buf)){
	fprintf(stderr, "Stat Error: %s: %s", dir_name, strerror(errno));
	exit (EXIT_FAILURE);
    }

    DIR * d;

    /* Open the directory specified by "dir_name". */

    d = opendir (dir_name);

    /* Check it was opened. */
    if (! d) {
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
    }
    while (1) {
        struct dirent * entry;
        const char * d_name;

        /* "Readdir" gets subsequent entries from "d". */
        entry = readdir (d);
        if (! entry) {
            /* There are no more entries in this directory, so break
               out of the while loop. */
            break;
        }

	int isDir=0;
        struct stat st_buf1;
        d_name = entry->d_name;
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
            /* Print the name of the file and directory. */
	    printf ("File: %s/%s\n", dir_name, d_name);

	}



        if ( isDir ) {

            /* Check that the directory is not "d" or d's parent. */
            
            if (strcmp (d_name, "..") != 0 &&
                strcmp (d_name, ".") != 0) {
                int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);
                //printf ("%s\n", path);
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
                /* Recursively call "list_dir" with the new path. */
                list_dir (path);
            }
	}
    }
    /* After going through all the entries, close the directory. */
    if (closedir (d)) {
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
