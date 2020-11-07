/*
 * File: storage/volume.h
 *
 * Descri��o:
 *     Gerenciador de volumes.
 *     Header para o Volume Manager.
 *     Ger�ncia de volumes.
 *
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2018 - Some new structures.
 */

 
#define VOLUME_COUNT_MAX 1024


#define VFS_VOLUME_ID              0
#define BOOTPARTITION_VOLUME_ID    1
#define SYSTEMPARTITION_VOLUME_ID  2
//...

//
// == system disk =================================================
//

// These are the main partitions 
// in the Gramado system.

// mbr
#define MBR_LBA               0 

// boot partition
#define VOLUME1_VBR_LBA       63
#define VOLUME1_FAT_LBA       67 
//#define VOLUME1_FAT2_LBA    ??
#define VOLUME1_ROOTDIR_LBA   559
#define VOLUME1_DATAAREA_LBA  591 

// system partition
#define VOLUME2_VBR_LBA       32000
#define VOLUME2_FAT_LBA       33000  
//#define VOLUME2_FAT2_LBA    ?? 
#define VOLUME2_ROOTDIR_LBA   34000
#define VOLUME2_DATAAREA_LBA  35000

// ==================================================================



char *current_volume_string;

// volume atual ??
// Tipo de sistema de arquivos, fat16, ext2 ...
int g_currentvolume_filesystem_type;   //use this one.


// volume atual do tipo fat???
// Se � fat32, 16, 12.
int g_currentvolume_fatbits;



/*
 *****************************************************
 * volume_type_t:
 *     Enumerando os tipos de volume.
 */

typedef enum {


    VOLUME_TYPE_NULL, 

    // Parti��o em disco f�sico.
    VOLUME_TYPE_DISK_PARTITION,  

    // Parti��o em disco virtual.
    VOLUME_TYPE_VIRTUAL_DISK_PARTITION,  

    // Arquivo.
    // Um arquivo qualquer. Sem formata��o.
    VOLUME_TYPE_RAW,           

    // Buffer.
    // Um arquivo qualquer. Sem formata��o.
    // Usado por banco de dados.
    VOLUME_TYPE_BUFFER,

    // Parti��o de swap.
    VOLUME_TYPE_SWAP

    //...

}volume_type_t;



/*
 * vbr_d:
 *     Structure for VBR parameters.
 */  

struct vbr_d
{
	//copiar mbr, � parecido;
}; 
struct vbr_d *vbr; 
 



/*
 * volume_d:
 *     Estrutura para acesso r�pido a volumes.
 *     Deve ser simples e com poucos elementos.
 */

struct volume_d
{
    object_type_t  objectType;
    object_class_t objectClass;

    int id;
    int used;
    int magic;
    
    // Only one thread can call read and write routine at time.
    int blocked;
    struct thread_d *waiting;  //this thread is waiting.

    // This is the process that call the read/write operation on this volume.
    pid_t pid;

    // See the enum.
    volume_type_t volumeType;

    // areas.
    // maybe we can find these in the superblock.
    // well, this is the fast access.
    unsigned long VBR_lba;
    unsigned long FAT1_lba;
    unsigned long FAT2_lba;
    unsigned long ROOT_lba;
    unsigned long DATA_lba;
    
    struct superblock_d super;



    // Ponteiro para um buffer se o tipo permitir.
    void *priv_buffer;
    unsigned long buffer_size_in_sectors;
    unsigned long sector_size;

    // Primeira lba se o tipo permitir.
    unsigned long __first_lba;
    
    // Last lba se o tipo permitir.
    unsigned long __last_lba;
    

    //filesystem_type_t filesystemType;


    //#todo
    // se est� funcionando ... se est� inicializado ...
    //int status;
    
    //#todo
    // que tipo de opera��o esta sendo realizada. ou nenhuma.
    // se ele est� ocupoado o escretor ter� que esperar.
    //int state;


    // Se � um volume virtual e precisa ser salvo
    // pois houve uma modifica��o.
    int need_to_save;

    // #todo
    // contador de processos usando o volume
     
    //
    // strings
    //


    //label
    char *name;

    //pathname
    char *cmd;

    //string usada no path para identificar o disco.
    //isso n�o existe.
    char path_string[32];  



    // Se o volume tiver um sistema de arquivos.
    // See: fs.h
    struct filesystem_d *fs;

    struct disk_d *disk;

    struct volume_d *next;
};

struct volume_d *volume_vfs;             // volume 0
struct volume_d *volume_bootpartition;   // volume 1
struct volume_d *volume_systempartition; // volume 2
// ...



//#importante:
//Esses s�o os tr�s volumes b�sicos do sistema 
//mesmo que o disco s� tenha um volume, essas 
//estruturas v�o existir.


//
// Lista de volumes.
//

unsigned long volumeList[VOLUME_COUNT_MAX];


//show info for all volumes in the list.
void volume_show_info (void);

void volumeShowCurrentVolumeInfo (void);

void *volume_get_volume_handle( int number );

void *volume_get_current_volume_info (void);

int volumeShowVolumeInfo( int descriptor );

int volume_init (void);


//
// End.
//
