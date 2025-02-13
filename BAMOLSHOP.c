#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define MAX_BUFFER 100
#define MAX_USERNAME_PASSWORD_INPUT 12
#define MAX_NAMA_PRODUK 30

#define AKUN_DATABASE "akunFile.txt"
#define PRODUK_DATABASE "produkFile.txt"


typedef struct {
   
   int id;
   char username[MAX_USERNAME_PASSWORD_INPUT];
   char password[MAX_USERNAME_PASSWORD_INPUT];
 
} account;

typedef struct {
   
   int id;
   char namaProduk[MAX_NAMA_PRODUK];
   int unit;
   int harga;
   int status;
   int milik;

} produk;

int inputString (char* variable, int batasChar, char elementTerlarang);
int inputUsernamePassword (char* variabel); 
int inputOpsi(int *variabel);
int ngitungJumlahAkun();
int ngitungJumlahProduk();
int loginPage (char *user, int *idUser);
int cekUsernameDiDatabase (const char *username);
int registerPage();
void halamanPertama();
int beliBarang(char* user, int idUser);
int jualBarang(int idUser);
void lihatProdukJualanSaya(char *user, int idUser);
void lihatProdukTersedia ();
void menuUtama (char* user, int idUser);

int main () {

   FILE *fileAkun = fopen(AKUN_DATABASE, "a");
   FILE *fileProduk = fopen(PRODUK_DATABASE, "a");

   fclose(fileAkun);
   fclose(fileProduk);

   printf("\n=============================================\n");
   printf("=====SELAMAT DATANG DI BAMMM ONLINE SHOP=====\n");
   printf("=============================================\n");
   halamanPertama();

}

int inputString (char* variable, int batasChar, char elementTerlarang) {

   char buffer[MAX_BUFFER];
   printf("input: ");

   if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

      buffer[strcspn(buffer, "\n")] = '\0';

      int status = 0;

      for (int i = 0; i < strlen(buffer); i++) {

         if (buffer[i] == elementTerlarang) {
            status = 1;
            break;
         }
      }

      if (status == 0 && strlen(buffer) <= batasChar) {

         strcpy(variable, buffer);
         return 1;

      } else {

         printf("WARNING: karakater maksimal %d, coba lagi\n\n", batasChar);
         return 0; 
      }
   
   } else {

      printf("ERROR: Input error, coba lagi!\n\n");
      return 0;
   }
}

int inputUsernamePassword (char* variabel) {

   char buffer[MAX_BUFFER];

   printf("input: ");

   if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

      buffer[strcspn(buffer, "\n")] = '\0';

      int status = 0;

      for (int i = 0; i < strlen(buffer); i++) {
         
         if (buffer[i] == ' ') {
            status = 1;
         }
      }

      if (status == 0 && strlen(buffer) <= MAX_USERNAME_PASSWORD_INPUT) {

         strcpy(variabel, buffer);
         return 1;

      } else {

         printf("WARNING: tidak boleh unsur space dan harus berkarakter 12\n\n");
         return 0;
      }
      
   } else {
 
      printf("ERROR: input error. coba lagi!\n\n");
      return 0;
   }
}

int inputOpsi(int *variabel) {

   char buffer[MAX_BUFFER];
   
   printf("Input: ");
      
   if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

      buffer[strcspn(buffer, "\n")] = '\0';
      int status = 0;

      for (int i = 0; i < strlen(buffer); i++) {

         if (!(isdigit(buffer[i]))) {

            status = 1;
         }
      }
         
      if (status == 0 && strlen(buffer) <= 2) {

         *variabel = atoi(buffer);
         return 1;
         
      } else {

         printf("WARNING: harus berupa maksimal dua digit, coba lagi!\n\n");
         return 0;
      }
      
   }  else {

      printf("ERROR: input sedang error, coba lagi!\n\n");
      return 0;
   }

}

int inputInteger(int *variabel, int batasDigit) {

   char buffer[MAX_BUFFER];
   
   printf("Input: ");
      
   if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

      buffer[strcspn(buffer, "\n")] = '\0';
      int status = 0;

      for (int i = 0; i < strlen(buffer); i++) {

         if (!(isdigit(buffer[i]))) {

            status = 1;
            break;
         }
      }
         
      if (status == 0 && strlen(buffer) <= batasDigit) {

         *variabel = atoi(buffer);
         return 1;
         
      } else {

         printf("WARNING: harus berupa maksimal dua digit, coba lagi!\n\n");
         return 0;
      }
      
   }  else {

      printf("ERROR: input sedang error, coba lagi!\n\n");
      return 0;
   }

}

int ngitungJumlahAkun() {

   int jumlah = 0;

   FILE *file = fopen(AKUN_DATABASE, "r");
   
   if (!file) {
      
      return 0;
   }

   account akun;
   
   while (fscanf(file, "%d %s %s",
      
      &akun.id, 
      akun.username, 
      akun.password
   
   ) != EOF) {

      jumlah++;
   }

   fclose(file);
   return jumlah;
}

int ngitungJumlahProduk() {

   int jumlah = 0;

   FILE *file = fopen(PRODUK_DATABASE, "r");
   
   if (!file) {
      
      return 0;
   }

   produk fileProduk;
   
   while (fscanf(file, "%d %s %d %d %d %d",
   
      &fileProduk.id, 
      fileProduk.namaProduk, 
      &fileProduk.unit, 
      &fileProduk.harga, 
      &fileProduk.status, 
      &fileProduk.milik
   
   ) != EOF) {

      jumlah++;
   }

   fclose(file);
   return jumlah;
}

int loginPage (char *user, int *idUser) {
   
   char username[MAX_USERNAME_PASSWORD_INPUT];
   char password[MAX_USERNAME_PASSWORD_INPUT];

   printf("\n===============\n");
   printf("=====LOGIN=====\n");
   printf("===============\n");
   
   printf("Masukkan username!\n");
   while(inputUsernamePassword(username) == 0);
   
   printf("masukkan password!\n");
   while(inputUsernamePassword(password) == 0);

   FILE * file = fopen(AKUN_DATABASE, "r"); 

   if (!file) {
      
      printf("database tidak ditemukan\n\n");
      exit(1);
   }

   account akun;

   while (fscanf(file, "%d %s %s",
      
      &akun.id, 
      akun.username, 
      akun.password
   
   ) != EOF) {

      if (strcmp(akun.username, username) == 0 && strcmp(akun.password, password) == 0) {
         
         printf("Login berhasil! selamat datang, %s!\n", username);   
         fclose(file);
         strcpy(user, akun.username);
         *idUser = akun.id;
         return 1;
      }
   }

   printf("ERROR: username atau password tidak valid!\n");
   fclose(file);
   halamanPertama();
   return 0;
}

int cekUsernameDiDatabase (const char *username) {

   FILE *file = fopen(AKUN_DATABASE, "r");
   
   if (!file) {

      printf("database tidak bisa di akses:(\n\n");
      return 0;
   }

   account akun;

   while(fscanf(file, "%d %s %s",
   
      &akun.id, 
      akun.username, 
      akun.password
   
   ) != EOF) {

      if (strcmp(akun.username, username) == 0) {
         
         fclose(file);
         return 0;
      }
   }

   fclose(file);
   return 1;
}

int registerPage() {

   account akunBaru;
   int jumlahAkun;

   printf("\n==================\n");
   printf("=====REGISTER=====\n");
   printf("==================\n");
   
   while(1) {
   
      printf("masukkan username anda!\n");
      while(inputUsernamePassword(akunBaru.username) == 0);
   
      if (cekUsernameDiDatabase(akunBaru.username) == 1) {

         break;
      }
      
      printf("username sudah digunakan, coba lagi!\n\n");
   }
   

   printf("masukkan password anda!\n");
   while(inputUsernamePassword(akunBaru.password) == 0);

   jumlahAkun = ngitungJumlahAkun();

   FILE *file = fopen(AKUN_DATABASE, "a");

   if (!file) {

      printf("ERROR: gagal membuka database, coba lagi!\n");
      return 0;
   }

   fprintf(file, "%d %s %s\n",
   
      jumlahAkun + 1, 
      akunBaru.username, 
      akunBaru.password
   );
   fclose(file);

   printf("Registrasi berhasil silahkan kembali kehalaman awal!\n\n");
   return 1;
}

void halamanPertama() {

   printf("selamat datang di BAMMM ONLINE SHOP\n");
   printf("sudah punya akun? => login(1)\n");
   printf("belum punya akun? => Register(2)\n");
   printf("keluar => (selain opsi atas)\n");

   int opsiMasukUser;
   char user[MAX_USERNAME_PASSWORD_INPUT];
   int idUser;

   while(inputOpsi(&opsiMasukUser) == 0);

   switch (opsiMasukUser) {

      case 1:  
         while(loginPage(user, &idUser) == 0);
         menuUtama(user, idUser); 
         break;
      case 2: 
         while(registerPage() == 0);
         halamanPertama(); 
         break;
      default: 
         printf("keluar dari BAMOLSHOP, terima kasih!\n\n"); 
         exit(0);
         break;
   }
}

int beliBarang(char* user, int idUser) {
   
   char fileStruk[MAX_BUFFER];
   
   if (ngitungJumlahProduk() == 0) {
      
      printf("produk saat ini belum tersedia!\n\n");
      return 0;
   }

   lihatProdukTersedia();

   int pilihanUser;
   printf("pilih ID produk yang ingin anda pilih!\n");
   while(inputOpsi(&pilihanUser) == 0);

   produk isiFileProduk;

   FILE *fileSimpanan = fopen("simpanan.txt", "w");
   
   if (!fileSimpanan) {
      printf("gagal membuat file simpanan :(");
      fclose(fileSimpanan);
      return 1;
   }

   FILE *fileProduk = fopen(PRODUK_DATABASE, "r+");

   if (!fileProduk) {
      
      printf("database sedang error :(");
      fclose(fileProduk);
      remove("simpanan.txt");
      return 1;
   }

   int status = 0;

   while(fscanf(fileProduk, "%d %s %d %d %d %d", 
   
      &isiFileProduk.id, 
      isiFileProduk.namaProduk, 
      &isiFileProduk.unit, 
      &isiFileProduk.harga, 
      &isiFileProduk.status, 
      &isiFileProduk.milik
   
   ) != EOF) {

      if(isiFileProduk.id == pilihanUser && isiFileProduk.unit > 0) {
         status = 1;
         int konfirmasi;

         printf("apakah anda yakin ingin membeli %s dengan harga %d? (1 => yakin, (selain 1) => tidak yakin)\n",
            isiFileProduk.namaProduk, isiFileProduk.harga
         );
         
         while(inputOpsi(&konfirmasi) == 0);

         if (konfirmasi != 1) {
            
            printf("kembali ke menu utama!\n\n");
            fclose(fileProduk);
            fclose(fileSimpanan);
            remove("simpanan.txt");
            return 1;
         
         }

         time_t waktuSekarang = time(NULL);
         struct tm *waktu = localtime(&waktuSekarang);

         snprintf(fileStruk, sizeof(fileStruk), "strukPembelian-%s--%02d-%02d-%04d--%02d-%02d-%02d.txt", 
            
            user,
            waktu->tm_mday, waktu->tm_mon + 1, waktu->tm_year + 1900,
            waktu->tm_hour, waktu->tm_min, waktu->tm_sec
         
         );

         FILE *fileStrukUser = fopen(fileStruk, "a");

         fprintf(fileStrukUser, "================================================\n");
         fprintf(fileStrukUser, "=============== Bukti Pembayaran ===============\n");
         fprintf(fileStrukUser, "================================================\n\n");

         fprintf(fileStrukUser, "ID user: %d\nNama User: %s\n\nID produk = %d\nnama produk = %s\nharga = Rp.%d\ntanggal Pembelian = %02d-%02d-%04d--%02d-%02d-%02d\n", 
            idUser, user,
            isiFileProduk.id, isiFileProduk.namaProduk, isiFileProduk.harga,
            waktu->tm_mday, waktu->tm_mon + 1, waktu->tm_year + 1900,
            waktu->tm_hour, waktu->tm_min, waktu->tm_sec
         );

         fprintf(fileStrukUser, "\n================================================\n");
         fprintf(fileStrukUser, "=================== BAMOLSHOP ==================\n");
         fprintf(fileStrukUser, "================================================");

         fclose(fileStrukUser);
         
         isiFileProduk.unit = isiFileProduk.unit - 1;
         
         if (!(isiFileProduk.unit > 0)) {
            
            isiFileProduk.status = 0;
         }
      } 

      fprintf(fileSimpanan, "%d %s %d %d %d %d\n", 
       
         isiFileProduk.id, 
         isiFileProduk.namaProduk, 
         isiFileProduk.unit, 
         isiFileProduk.harga, 
         isiFileProduk.status, 
         isiFileProduk.milik
      );
   }
   
   fclose(fileProduk);
   fclose(fileSimpanan);

   if (status == 1) {
      
      remove(PRODUK_DATABASE);
      rename("simpanan.txt", PRODUK_DATABASE);
      printf("\npembelian anda berhasil!\n\n");
      return 1;
   
   } else {

      printf("gagal pembelian, coba lagi!\n\n");
      printf("tidak menemukan ID produk atau stok produk sudah habis!\n\n"); 
      remove("simpanan.txt");
      return 0;
   }
}

int jualBarang(int idUser) {

   produk produkSaya;
   int jumlahProduk = ngitungJumlahProduk();

   printf("masukkan nama produk anda (tidak boleh ada spacing)!\n");
   while(inputString(produkSaya.namaProduk, 30, ' ') == 0);

   printf("masukkan unit produk yang tersedia(tidak boleh 0)!\n");
   while(inputInteger(&produkSaya.unit, 3) == 0);

   printf("masukkan harga per unit!\n");
   while(inputInteger(&produkSaya.harga, 9) == 0);

   FILE *file = fopen(PRODUK_DATABASE, "a");

   if (!file) {
      printf("tidak bisa membuka file produk :(\n");
      return 0;
   }

   fprintf(file, "%d %s %d %d %d %d\n", 
   
      jumlahProduk + 1, 
      produkSaya.namaProduk, 
      produkSaya.unit, 
      produkSaya.harga, 
      1, 
      idUser
   );

   fclose(file);

   printf("produk anda sudah masuk ke dalam penjualan online!\n\n");
   return 1;
}

void lihatProdukJualanSaya(char *user, int idUser) {

   FILE *fileProduk = fopen(PRODUK_DATABASE, "r");

   produk isiFileProduk;

   printf("\nPRODUK : %s\n", user);

   printf("\n%-5s | %-30s | %-6s | %-14s | %-14s \n", 
           "ID", "Nama Produk", "Unit", "Harga", "Status", "Milik");
   printf("===================================================================================\n");

   while(fscanf(fileProduk, "%d %s %d %d %d %d", 
   
      &isiFileProduk.id, 
      isiFileProduk.namaProduk, 
      &isiFileProduk.unit, 
      &isiFileProduk.harga, 
      &isiFileProduk.status, 
      &isiFileProduk.milik
   
   ) != EOF) {

      if (isiFileProduk.milik == idUser) {

         printf("%-5d | %-30s | %-6d | Rp. %-10d | %-14s \n",
   
            isiFileProduk.id,
            isiFileProduk.namaProduk,
            isiFileProduk.unit,
            isiFileProduk.harga,
            isiFileProduk.status ? "tersedia" : "tidak tersedia"
         );
      }
   }
   printf("\n");

   fclose(fileProduk);

}

void lihatProdukTersedia () {

   FILE *file = fopen(PRODUK_DATABASE, "r");
   

   account akun;

   produk daftarProduk;

   printf("\n%-5s | %-30s | %-6s | %-14s | %-14s | %-10s\n", 
           "ID", "Nama Produk", "Unit", "Harga", "Status", "Milik");
   printf("============================================================================================\n");

   while (fscanf(file, "%d %s %d %d %d %d",
      
      &daftarProduk.id, 
      daftarProduk.namaProduk, 
      &daftarProduk.unit, 
      &daftarProduk.harga, 
      &daftarProduk.status, 
      &daftarProduk.milik

   ) != EOF) {

      char milik[100];

      FILE *akunFile = fopen(AKUN_DATABASE, "r");

      while (fscanf(akunFile, "%d %s %s", 
      
         &akun.id, 
         akun.username, 
         akun.password
      
      ) != EOF) {

         if (daftarProduk.milik == akun.id) {

            strcpy(milik, akun.username);
            break;
         }
      }
      
      printf("%-5d | %-30s | %-6d | Rp. %-10d | %-14s | %-10s\n",
         daftarProduk.id, 
         daftarProduk.namaProduk, 
         daftarProduk.unit, 
         daftarProduk.harga, 
         daftarProduk.status ? "tersedia" : "tidak tersedia", 
         milik
      );
      fclose(akunFile);
   }

   printf("\n");

   fclose(file);
}

void menuUtama (char* user, int idUser) {
   
   int opsiUser;
   int status = 1;
   
   printf("\n====================\n");
   printf("=====MENU UTAMA=====\n");
   printf("====================\n");
   printf("haloo, %s\n", user);

   while (status) {
      
      printf("1. Beli barang\n");
      printf("2. Jual barang\n");
      printf("3. Lihat produk tersedia online\n");
      printf("4. Lihat produk jualan saya\n");
      printf("5. Logout akun\n");
      printf("6. keluar app\n");
   
      printf("pilih opsi!\n");
      while(inputOpsi(&opsiUser) == 0);
      
      switch (opsiUser) {

         case 1: 
            beliBarang(user, idUser); 
            break;
         case 2: 
            while(jualBarang(idUser) == 0); 
            break;
         case 3: 
            lihatProdukTersedia(); 
            break;
         case 4: 
            lihatProdukJualanSaya(user, idUser); 
            break;
         case 5: 
            status = 0; 
            halamanPertama(); 
            break;
         case 6: 
            status = 0; 
            printf("Terima kasih sudah menggunakan service kami, sampai jumpa nanti!!!!!\n");
            exit(0);
            break;
         default: 
            printf("input tidak valid, coba lagi(1-8)!\n"); 
            break;
      }
   }
}