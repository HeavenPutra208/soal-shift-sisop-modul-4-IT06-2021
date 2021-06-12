# soal-shift-sisop-modul-4-IT06-2021

Penyelesaian Soal Shift 4 Sistem Operasi 2021\
Kelompok IT06
* Alessandro Tionardo (05311940000018)
* Heaven Happyna P F (0531940000026)
* Zuhairaja M T (05311940000033)

---
## Daftar Isi
* [Soal 1](#soal-1)
* [Soal 2](#soal-2)
* [Soal 3](#soal-3)
* [Soal 4](#soal-4)
---

## Soal 1
**Penyelesaian**
soal meminta kita untuk membuat sebuah metode enkripsi atbash yang akan terjadi jika sebuah direktori itu di-**`mkdir`**  dan di-**`rename`** dengan awalan `AtoZ_`, yang dimana setiap file atau direktori yang berada di dalam direktori baru akan dienkripsi dengan atbash. Jika direktori dengan awalan `AtoZ_` direname menjadi tidak ternecode atau dengan awalan `AtoZ_` maka semua isi dari direktori akan terdekripsi. Filesystem yang telah ditentukan adalah direktori ` /home/[USER]/Downloads`.

**Pembahasan**
Kami membuat 3 buah fungsi yang dapat membantu pengerjaan soal nomor 1 yang berupa fungsi `decrypt`, `getDirAndFile`, dan `ChangePath`. `decrypt` berfungsi untuk melakukan enkripsi dan dekripsi atbash, `getDirAndFile` untuk mendapatkan direktori dan file dari path yang telah ditentukan, dan `ChangePath` digunakan untuk menganti path yang ditentukan, dan juga digunakan untuk mengecek apakah path yang diganti diperlukan melakukan dekripsi ataupun enkripsinya.

**Fungsi decrypt**
```c
void decrypt(char *s) {
    for (int i = 0; i < strlen(s); i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = 'Z'+'A'-s[i];
        else if ('a' <= s[i] && s[i] <= 'z') s[i] = 'z'+'a'-s[i];
    }
}
```
Fungsi `decrypt` menerima 1 argument yang berupa `s`. Char `s` dapat menggunakan metode enkripsi dan juga dekripsi pada `for` loop yang terdapat pada fungsi, dimana pada `for` loop memiliki algoritma atbash yang dapat melakukan enkripsi dan juga dekripsi.

**Fungsi getDirAndFile**
```c
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
```
Fungsi `getDirAndFile` akan membagi path yang diinputkan menjadi `dir` dan  `file` berdasarkan lokasi `/` terakhir. Fungsi pertama-tama mendeklarasi variable `buff` untuk menyimpan `path` agar nilai `path` tidak berubah saat dilakukan `strtok()`. Lalu `dir` dan `file` akan di `memset()` agar isinya menjadi kosong.  Lalu `path` akan di-`strcpy()` kedalam `buff`. Selanjutnya akan dibuat sebuah `token` dengan `strtok()` pada `buff` bersama delimiter `"/"`, lalu akan dilakukan iterasi untuk masing-masing `token` yang ada. Setiap iterasi akan mengisi isi `token` kedalam variable `file` menggunakan `sprintf()`, lalu token akan diiterasi ke token selanjutnya. Di akhir akan dicek apakah token tersebut merupakan token terakhir atau bukan, jika bukan, maka variable `dir` akan di update menjadi `dir/file` menggunakan `sprintf()`.

**Fungsi changePath**
```c
void changePath(char *fpath, const char *path, int isWriteOper, int isFileAsked) {
  char *ptr = strstr(path, "/AtoZ_");
  int state = 0;
  if (ptr != NULL) {
    if (strstr(ptr+1, "/") != NULL) state = 1;
  }
  ...
}
```
Variable `*ptr` digunakan untuk menunjuk posisi `/AtoZ_` pada `path` menggunakan fungsi `strstr()`. Selanjutnya akan deklarasi variable `state` untuk memastikan apakah `path` yang diinputkan berisi directory didalamnya atau hanya `/AtoZ_`. Lalu akan dicek apakah string setelah karakter pertama dari `ptr` memiliki karakter `/`. Jika iya, maka state akan diset menjadi `1`.
```c
  char fixPath[1000];
  memset(fixPath, 0, sizeof(fixPath));
  if (ptr != NULL && state) {
    ptr = strstr(ptr+1, "/");
    char pathEncvDirBuff[1000];
    char pathEncryptedBuff[1000];
    strcpy(pathEncryptedBuff, ptr);
    strncpy(pathEncvDirBuff, path, ptr-path);
    ...
  }
```
Selanjutnya, mendefinisikan buffer `fixpath[]` yang akan digunakan untuk menyimpan proccessed path untuk setiap kondisi yang berbeda. `if()` disini akan berjalan ketika terdapat substring `/AtoZ_` pada `path` dengan kondisi `state` **true**. lalu `ptr` akan diset menjadi path setelah `/AtoZ_`. Lalu akan didefinisikan `pathEncryptedBuff` untuk menyimpan path `*ptr` yang telah dicari menggunakan fungsi `strcpy()`, dan juga didefinisikan `pathEncvDirBuff` untuk menyimpan `path` yang tidak didekripsi menggunakan fungsi `strncpy()` dari karakter pertama `path` sampai panjang `ptr-path`.
```c
    if (isWriteOper) {
      char pathFileBuff[1000];
      char pathDirBuff[1000];
      getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
      decrypt(pathDirBuff, 0);
      sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
    } else if ...
```
Dalam kondisi **`isWriteOper`**, yang didekripsi hanya path directory saja. Sehingga `pathEncryptedBuff` perlu dipisah menggunakan fungsi `getDirAndFile()` dan disimpan pada `pathDirBuff` dan `pathFileBuff`. Lalu akan menjalankan fungsi `decrypt()` pada `pathDirBuff` saja. Setelah itu, seluruh path akan dijadikan satu kedalam buffer `fixPath` dengan urutan `pathEncvDirBuff` (path tidak terenkripsi), `pathDirBuff` (path yang dienkripsi), dan terakhir `pathFileBuff` (nama file).
```c
    } else if (isFileAsked) {
      char pathFileBuff[1000];
      char pathDirBuff[1000];
      char pathExtBuff[1000];
      getDirAndFile(pathDirBuff, pathFileBuff, pathEncryptedBuff);
      char *whereIsExtension = strrchr(pathFileBuff, '.');
      if (whereIsExtension-pathFileBuff < 1) {
        decrypt(pathDirBuff, 0);
        decrypt(pathFileBuff, 0);
        sprintf(fixPath, "%s%s/%s", pathEncvDirBuff, pathDirBuff, pathFileBuff);
      } else {
        char pathJustFileBuff[1000];
        memset(pathJustFileBuff, 0, sizeof(pathJustFileBuff));
        strcpy(pathExtBuff, whereIsExtension);
        snprintf(pathJustFileBuff, whereIsExtension-pathFileBuff+1, "%s", pathFileBuff);
        decrypt(pathDirBuff, 0);
        decrypt(pathJustFileBuff, 0);
        sprintf(fixPath, "%s%s/%s%s", pathEncvDirBuff, pathDirBuff, pathJustFileBuff, pathExtBuff);
      }
    } else ...
```
Bagian ini berjalan untuk kondisi dimana bukan `isWriteOper` dan yang diminta merupakan `isFileAsked`. Pertama-tama, path `directory` dan path `file` dipisah dahulu menggunakan fungsi `getDirAndFile()`. Lalu untuk mencari ekstensi, akan dideklarasi pointer `*whereIsExtension` untuk menunjuk karakter '.' terakhir pada `pathFileBuff` menggunakan fungsi `strrchr()`. Lalu dicek apakah karakter '.' terakhir ini merupakan karakter pertama.

Jika iya, maka `pathDirBuff` dan `pathFileBuff` akan didekripsi menggunakan fungsi `decrypt()` dan disatukan kembali pada variable `fixPath` sesuai formatnya. Jika ternyata terdapat ekstensi, maka akan dideklarasi `pathJustFileBuff` untuk menyimpan nama filenya saja menggunakan `snprintf` dan mengisi value `pathExtBuff` menjadi ekstensinya saja. Lalu `pathDirBuff` dan `pathJustFileBuff` akan didekripsi menggunakan fungsi `decrypt()` dan disatukan kembali pada variable `fixPath` sesuai formatnya.
```c
    } else {
      decrypt(pathEncryptedBuff, 0);
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
```
`else` pertama disini untuk kondisi bukan `isWriteOper` dan yang diminta bukan `isFileAsked` atau *directory* sehingga isi dalam buffer `pathEncryptedBuff` akan langsung didekripsi dan disimpan kedalam `fixPath`. `else` kedua adalah untuk kondisi `state == 0` atau tidak ada karakter `AtoZ_` maka isi dari `fixPath` akan sama dengan `path` karena tidak ada metode enkripsi dan dekripsi yang akan dijalankan. Selanjutnya adalah mengubah `fixPath` menjadi `fpath` dengan mengisi **`mount-point`** yang telah dideklarasi pada variable `dirpath`. 

**system-call readdir**
```c
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
```
*System-call* `xmp_readdir` ini digunakan untuk melakukan enkripsi pada entry yang berada pada directory `/AtoZ_`. Pada fungsi diatas, jika pada path terdapat `/AtoZ_`, maka akan dilakukan enkripsi pada isi pathnya dengan fungsi `decrypt` Jika `path` yang dimasukkan tidak memiliki `/AtoZ_` didalamnya, maka nama entry `de->d_name` akan langsung dimasukkan kedalam buf menggunakan fungsi `filler()`.

**system-call mkdir**
```c
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
```
Pada contoh diatas, merupakan *system-call* `_mkdir()`. Pada *system-call* tersebut, akan dilakukan `changePath()` terhadap `path` untuk mendapatkan `fpath`, arguments `isWriteOper = 1` karena `mkdir` merupakan operasi *write* (membuat).

**system-call getattr**
```c
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
```
Pada contoh diatas, merupakan *system-call* `_getattr()`. Pada *system-call* tersebut, akan dilakukan `changePath()` terhadap `path` untuk mendapatkan `fpath`. Karena ketika `path` yang dimasukkan kita tidak tahu apakah file tersebut merupakan file biasa atau merupakan directory maka kita akan mengubah terlebih dahulu dengan asumsi bahwa `path` tersebut merupakan file, sehingga argument `isFileAsked = 1`. Lalu akan dicoba di `access` apakah bisa, jika tidak bisa diakses, maka akan dilakukan changePath kembali dengan argument `isFileAsked = 0` (directory). Lalu proses `getattr` akan berlangsung pada `fpath` tersebut.

### Screenshot
**Hasil Running Code**

### Kendala


## Soal 2
**Penyelesaian**
Mohon maaf soal ini tidak kami kerjakan.
### Screenshot
**Hasil Running Code**
### Kendala

## Soal 3
**Penyelesaian**
Mohon maaf soal ini tidak kami kerjakan.
### Screenshot
**Hasil Running Code**
### Kendala

## Soal 4
**Penyelesaian**

### Screenshot
**Hasil Running Code**

### Kendala
