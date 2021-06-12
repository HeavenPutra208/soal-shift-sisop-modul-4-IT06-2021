#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirpath = "/home/aless/Downloads";
static const char *logpath = "/home/aless/SinSeiFS.log";

void decrypt(char *s) {
    for (int i = 0; i < strlen(s); i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'Z'+'A'-s[i];
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'z'+'a'-s[i];
    }
}

void getDirAndFile(char *dir, char *file, char *path) {
  char buff[1000];
  memset(dir, 0, 1000);
  memset(file, 0, 1000);
  strcpy(buff, path);
  char *token = strtok(buff, "/");
  while(token != NULL) {
    sprintf(file, "%s", token);
    token = strtok(NULL, "/");
    if (token != NULL) {
      sprintf(dir, "%s/%s", dir, file);
    }
  }
}

void changePath(char *fpath, const char *path, int isWriteOper, int isFileAsked) {
  char *ptr = strstr(path, "/AtoZ_");
  int state = 0;
  if (ptr != NULL) {
    if (strstr(ptr+1, "/") != NULL) state = 1;
  }
  char fixPath[1000];
  memset(fixPath, 0, sizeof(fixPath));
  if (ptr != NULL && state) {
    ptr = strstr(ptr+1, "/");
    char pathEncvDirBuff[1000];
    char pathEncryptedBuff[1000];
    strcpy(pathEncryptedBuff, ptr);
    strncpy(pathEncvDirBuff, path, ptr-path);
    if (isWriteOper) {
      char pathFileBuff[1000];
      char pathDirBuff[1000];
      getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
      decrypt(pathDirBuff);
      sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
    } else if (isFileAsked) {
      char pathFileBuff[1000];
      char pathDirBuff[1000];
      char pathExtBuff[1000];
      getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
      char *whereIsExtension = strrchr(pathFileBuff, '.');
      if (whereIsExtension-pathFileBuff < 1) {
        decrypt(pathDirBuff);
        decrypt(pathFileBuff);
        sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
      } else {
        char pathJustFileBuff[1000];
        memset(pathJustFileBuff, 0, sizeof(pathJustFileBuff));
        strcpy(pathExtBuff, whereIsExtension);
        snprintf(pathJustFileBuff, whereIsExtension-pathFileBuff+1, "%s", pathFileBuff);
        decrypt(pathDirBuff);
        decrypt(pathJustFileBuff);
        sprintf(fixPath, "%s%s/%s%s", pathEncvDirBuff, pathDirBuff, pathJustFileBuff, pathExtBuff);
      }
    } else {
      decrypt(pathEncryptedBuff);
      sprintf(fixPath, "%s%s", pathEncvDirBuff, pathEncryptedBuff);
    }
  } else {
    strcpy(fixPath, path);
  }
  if (strcmp(path, "/") == 0) {
    sprintf(fpath, "%s", dirpath);
  } else {
    sprintf(fpath, "%s%s", dirpath, fixPath);
  }
}

void logFile(char *level, char *cmd, int res, int lenDesc, const char *desc[]) {
  FILE *f = fopen(logpath, "a");
  time_t t;
  struct tm *tmp;
  char timeBuff[100];

  time(&t);
  tmp = localtime(&t);
  strftime(timeBuff, sizeof(timeBuff), "%d%m%y-%H:%M:%S", tmp);

  fprintf(f, "%s::%s::%s::%d", level, timeBuff, cmd, res);
  for (int i = 0; i < lenDesc; i++) {
    fprintf(f, "::%s", desc[i]);
  }
  fprintf(f, "\n");

  fclose(f);
}

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{

	char fpath[1000];
  changePath(fpath, path, 0, 1);
  if (access(fpath, F_OK) == -1) {
    memset(fpath, 0, sizeof(fpath));
    changePath(fpath, path, 0, 0);
  }

	int res;

	res = lstat(fpath, stbuf);

  const char *desc[] = {path};
  logFile("INFO", "GETATTR", res, 1, desc);

	if (res == -1) return -errno;


	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	changePath(fpath, path, 0, 0);

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL) {
    const char *desc[] = {path};
    logFile("INFO", "READDIR", -1, 1, desc);
    return -errno;
  }

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
    if (strstr(path, "/AtoZ_") != NULL) {
      char encryptThis[1000];
      strcpy(encryptThis, de->d_name);
      if (de->d_type == DT_REG) {
        char *whereIsExtension = strrchr(encryptThis, '.');
        if (whereIsExtension-encryptThis < 1) {
          decrypt(encryptThis);
        } else {
          char pathFileBuff[1000];
          char pathExtBuff[1000];
          strcpy(pathExtBuff, whereIsExtension);
          snprintf(pathFileBuff, whereIsExtension-encryptThis+1, "%s", encryptThis);
          decrypt(pathFileBuff);
          memset(encryptThis, 0, sizeof(encryptThis));
          sprintf(encryptThis, "%s%s", pathFileBuff, pathExtBuff);
        }
      } else if (de->d_type == DT_DIR) {
        decrypt(encryptThis);
      }

  		if (filler(buf, encryptThis, &st, 0)) break;
    } else {
      if (filler(buf, de->d_name, &st, 0)) break;
    }
	}

  const char *desc[] = {path};
  logFile("INFO", "READDIR", 0, 1, desc);

	closedir(dp);
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	char fpath[1000];
	changePath(fpath, path, 1, 0);

  char *ptr = strrchr(path, '/');
  char *filePtr = strstr(ptr, "/AtoZ_");
  if (filePtr != NULL) {
    if (filePtr - ptr == 0) {
      const char *desc[] = {path};
      logFile("SPECIAL", "AtoZ", 0, 1, desc);
    }
  }

	int res;

	res = mkdir(fpath, mode);

	if (res == -1) return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	char ffrom[1000];
	char fto[1000];
	changePath(ffrom, from, 0, 1);
	changePath(fto, to, 0, 1);
  if (access(ffrom, F_OK) == -1) {
    memset(ffrom, 0, sizeof(ffrom));
    changePath(ffrom, from, 0, 0);
  }
  if (access(fto, F_OK) == -1) {
    memset(fto, 0, sizeof(fto));
    changePath(fto, to, 0, 0);
  }

  char *toPtr = strrchr(to, '/');
  char *fromPtr = strrchr(from ,'/');
  char *toStartPtr = strstr(toPtr, "/encv2_");
  char *fromStartPtr = strstr(fromPtr, "/encv2_");
 
  toStartPtr = strstr(toPtr, "/AtoZ_");
  if (toStartPtr != NULL) {
    if (toStartPtr - toPtr == 0) {
      const char *desc[] = {fto};
      logFile("SPECIAL", "AtoZ", 0, 1, desc);
    }
  }

	int res;

	res = rename(ffrom, fto);

  const char *desc[] = {from, to};
  logFile("INFO", "RENAME", res, 2, desc);

	if (res == -1) return -errno;

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;

        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .mkdir = xmp_mkdir,
    .rename = xmp_rename,
};

int  main(int  argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
