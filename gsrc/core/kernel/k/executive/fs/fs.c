/*
 * File: fs\fs.c 
 *
 * Descri��o:
 *    Arquivo principal do file system manager.
 *    M�dulo em kernel mode, incluido no kernel base. 
 *    
 *    *IMPORTANTE: A id�ia principal � que esse m�dulo de o suporte ao acesso 
 * a qualquer sistema de arquivo suportado, esteja ele em user mode ou em 
 * kernel mode.
 *
 *    Arquivo principal do m�dulo fs do executive do kernel..
 *    Gerenciador de arquivos do kernel.
 *    Na verdade o Kernel n�o tem atribui��es de carregar arquivos. Mas
 * ele ofere�e servi�os b�sicos que podem ser chamados pelos aplicativos.
 *
 * Objetivo:
 *    Ofere�er opera��es nos sistemas de arquivos suportados pelo kernel.
 *    Esse gerenciador chama as fun��es em kernel mode do sistema de arquivos
 * interno e chama as servi�os oferecidos pelos drivers em user mode.
 *
 *    Os drivers em user mode podem oferecer acesso � sistemas de arquivos
 * variados.
 *
 *   Ao kernel, compete as fun��es b�sicas de opera��es com hardware.
 *   O sistema de arquivos suportado pelo kernel � fat16.
 *   Continua ...
 *
 * Observa��o: 
 *    Os valores das estruturas do sistema de arquivos podem ser salvos 
 * em um registro.
 *    No registro pode ter um registro das opera��es efetuadas 
 * no sistema de arquivo.
 *   Continua ...
 *   
 * Hist�rico:
 *    Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *    Vers�o 1.0: 2016 - Revis�o.
 *    ...
 */
 

#include <kernel.h>




//Vari�veis internas.
//int fsStatus;
//int fsError;
//...


				  
void fsListFiles( int disk_id, 
                  int volume_id, 
				  int directory_id )
{
	printf("fsListFiles: disk={%d} vol={%d} dir={%d}\n", 
	    disk_id, volume_id, directory_id);

    //
	// @todo: Checar mais limites.
	//
	
	if( disk_id == -1 || 
	    volume_id == -1 || 
		directory_id == -1 )
	{
		goto fail;	
	}
	
	
	//
	// Show!
	//
	
	
	//
	// Se o diret�rio selecionado � o diret�rio raiz 
	// do VFS.
	//
	
	if( current_disk == 0 && 
	    current_volume == 0 &&
		current_directory == 0 )
	{
	    vfsListFiles();
        goto done;		
	};
	
	// @todo: Suportar outros diret�rios.
	// ...
	
fail:
    printf("fail\n");
done:	
	refresh_screen();
};


/*
 * fsCreateVFS:
 * 
 * #bugbug : Isso est� errado:
 * agora temos rotinas de vfs em vfs.c
 *
  @todo
//  
// ** CRIANDO O DIRET�RIO RAIZ DO VFS BASEADO EM STREAMS **
//

//criando o diret�rio '/vfs'

**/
void fsCreateVFS()
{
/*	
    FILE *vfs;
    char *vfs_root;
  
    //inicializa a estrutura ...
    vfs = ( void* ) malloc( sizeof(struct _iobuf) );
	if( (void*) vfs == NULL  ){
		//die();
	}	
	
	//aloca mem�ria para o diret�rio raiz do vfs.
    vfs_root = (char *) malloc(512*16);
	
	if( (char *) vfs_root == (char *) 0 )
	{
		//die();
	}else{
		
		//O in�cio da �rea de mem�ria reservado para o diret�rio 
		//ser� salvo na estrutura de stream.
		vfs->_base = (char *) vfs_root;
        //...
	};
*/
};





/*
void fsClearRootdirBuffer();
void fsClearRootdirBuffer()
{
    limpa_root();
	return;
}
*/



/*
void fsClearFatBuffer();
void fsClearFatBuffer()
{
    limpa_fat();
	return;
}
*/


/*
int fsIsFAT12();
int fsIsFAT12()
{
   //testa o tipo de fat.
    return 0;
};
*/

/*
int fsIsFAT16();
int fsIsFAT16()
{
    //testa o tipo de fat.
    return 0;
};
*/

/*
int fsIsFAT32();
int fsIsFAT32()
{
    //testa o tipo de fat.
    return 0;
};
*/


/*
int fs_open()
int fs_open()
{
  
    if( fs_channel == NULL)
	{
	    return 1;
	}
	else
	{
        fs_channel->opened = 1;		
	};
	
done:
    return;	
};
*/


/*
 * fsListFiles:
 *     Mostra os nomes dos arquivos de um diret�rio.
 *     Sistema de arquivos fat16.
 *
 * IN:
 *     dir = Ponteiro para um endere�o de mem�ria onde foi carregado 
 * o diret�rio.
 *
 */
void fsFAT16ListFiles( const char *dir_name, 
                       unsigned short *dir_address, 
					   int number_of_entries )
{
	//@todo: O n�mero de entradas � maior no diret�rio raiz.(512 ?)
	
    int i = 0;
	int max = number_of_entries;         // N�mero m�ximo de entradas.
    
	unsigned long j = 0;  // Deslocamento
	
	unsigned short *DirBaseAddress = (unsigned short *) dir_address;
	
	
	if( number_of_entries <= 0 ){
		return;
	}
	
	printf("fsListFiles: Listing names in %s ...\n", dir_name);
	
    
	// Mostra.
	i=0; 
	while(i < max)
	{
		// Diferente de vazio.
		if( DirBaseAddress[j] != 0 )
		{
			//O problema � a termina��o da string '\0'
			printf( "%s\n", &DirBaseAddress[j] );
        } 
		
		//(32/2) pr�xima entrada! (16 words) 512 vezes!
        j += 16;    
        ++i;        
    }; 
	
	//...
	
	//printf("fsListFiles: done\n");
	printf("Done\n");	
	return; 
};



/*
 * KeLoadFile:
 *     Carraga um arquivo na mem�ria.
 *     @todo: Ke_ n�o � o certo, pois ke � chamadas 
 *            � rotinas externas. 
 *
 *  #bugbug a estrutura de canal ser� usada para troca de mensagens S|----|S
 */
int KeLoadFile( struct channel_d *channel, 
                unsigned char *file_name, 
				unsigned long file_address )
{
    unsigned long Status;
	
	//validate
	if((void*) channel == NULL){
	    return (int) 1;
	}else{
	    
		//Aberto?
	    if(channel->State != 1){
		    return (int) 1;
		};
		
		//esta em uso.
		channel->InUse = 1;
	
	};	
	
	//confere se o canal passado por argumento est� aberto.
	
    Status = fsLoadFile(file_name,file_address);
	if( Status != 0)
	{
	    channel->InUse = 0;
		channel->State = 0;
	    return (int) 1;
	};
	
	//N�o est� mais em uso.
	channel->InUse = 0;
	
	//Close channel.
	channel->State = 0;
	
done:
    return (int) 0;	
};



/*
 * get_file:
 *     Obt�m o ponteiro da estrutura do arquivo, dado seu id.
 *     Com um �ndice da lista de arquivos, a fun��o retorna o ponteiro da
 *     estrutura do arquivo.
 */
void *get_file(int Index)
{
	//Limits.	
	//@todo: max.
	if(Index < 0){
	    return NULL;
	};
    return (void *) fileList[Index];
};


/*
 * set_file:
 *     Coloca na lista de arquivos o ponteiro de estrutura,
 * dado seu id.
 * 
 */
void set_file( void *file, int Index )
{
	//Limits.	
	if(Index < 0){
	    return;
	};
	
	//
	// @todo: Limite m�ximo da lista.
	//
	
    // Structure.
	if( (void*) file == NULL){
		return;
	};	
	
	// Include pointer in the list.
	fileList[Index] = (unsigned long) file;
	return;
};


void fs_test_fat_vector(){
    return; //Ainda n�o implementada.
};


unsigned long fs_get_fat_entry(unsigned long n){	
    return 0; //Ainda n�o implementada.
};
											 

void fs_set_fat_entry(unsigned long n, unsigned long value){	
    return;	//Ainda n�o implementada.
};


void fs_put_list_on_fat(){
    return; //Ainda n�o implementada.
};


void fs_set_structures()
{
    return; //Ainda n�o implementada.
};


void fs_set_entry(unsigned long id, unsigned long eid)
{
    return; //suspensa
};


void fs_get_entry(unsigned long id, unsigned long eid)
{
    return; //suspensa
};


void fs_show_dir_entry(unsigned long id,unsigned long eid){	
	return; //Ainda n�o implementada.
};



void fs_show_dir(unsigned long id){
    return; //Ainda n�o implementada.
};


//confere clusers de um diretorio.
unsigned long fs_check_cluster(unsigned long id){
	return 0; //Ainda n�o implementada.
};



/*
 * fs_check_fat:
 *     Check FAT.
 */
unsigned long fs_check_fat(){
	return 0; //Ainda n�o implementada.
};



void fs_show_entry(unsigned long id, unsigned long eid){
	return; //Ainda n�o implementada.
};


unsigned long fs_get_entry_status(unsigned long id, unsigned long eid){
	return 0; //Ainda n�o implementada.
};


void fs_set_entry_status(unsigned long id, unsigned long eid, unsigned long status){
    return; //Ainda n�o implementada.
};


/*
 **************************************************************************
 * fsCheckMbr:
 *     Checamos um mbr carregado em algum endere�o de mem�ria.
 *
 *     Checa o registro do disco. (MBR). (disco do sistema).
 *
 *     Obs: O MBR desse sistema n�o tem o bpb no inicio do primeiro setor e 
 * sim no final do primeiro setor, antes da tabela de parti��es.
 *
 * Endere�o da mem�ria onde o MBR � carregado na inicializa��o. 0x00020000.   
 *
 * @todo: Talvez essa rotina pudesse fornecer o endere�o do buffer como 
 * argumento.
 * @todo: O mbr do sisco do sistema precisa de uma estruura que 
 * coordene o acesso a ele.
 */
//void fsCheckMBR(unsigned char* buffer)  //@todo
void fsCheckMbrFile( unsigned char *buffer )
{
	//Onde carregar.
	unsigned char *mbr = (unsigned char *) buffer; 

    //
	// @todo:
	// Checar uma estrutura do mbr do disco do sistema,
	// para validar o acesso � ele.
	//
	
	
	// Check signature.
	if( mbr[0x1FE] != 0x55 || mbr[0x1FF] != 0xAA )
	{
	    printf("fsCheckMbr: Signature Fail!\n");
        goto fail;		
	};
	
	//
	// Continua ...
	//
	
done:
    printf("fsCheckMbr: Done.\n");
	refresh_screen();
	return;
	
fail:
    refresh_screen();
    return;
};


/*
 * fsCheckVbr:
 *     Checa o registro de boot de um volume.
 *     Vai no endere�o onde est� armazenado o VBR do volume atual
 *     e confere as informa��es sobre o volume.
 */
void fsCheckVbrFile( unsigned char *buffer )
{
	//Onde carregar.
	unsigned char *mbr = (unsigned char *) buffer; 

    //
	// @todo:
	// Checar uma estrutura do mbr do disco do sistema,
	// para validar o acesso � ele.
	//
	
	
	// Check signature.
	if( mbr[0x1FE] != 0x55 || mbr[0x1FF] != 0xAA )
	{
	    printf("fsCheckMbr: Signature Fail!\n");
        goto fail;		
	};
	
	//
	// Continua ...
	//
	
done:
    printf("fsCheckMbr: Done.\n");
	refresh_screen();
	return;
	
fail:
    refresh_screen();
    return;
};



/*
 * fs_check_disk:
 *    Checa elementos do disco.
 *    Disco, Volume.
 *    Mbr. Vbr.
 * @todo: Mudar para fsCheckDisk().
 */
void fs_check_disk()
{
    printf("fs_check_disk: Initializing..\n");	
	//fsCheckMbrFile();
	//fsCheckVbrFile();
    //...
	
// Done.
done:
    printf("Done.\n");
	//refresh_screen();
    return;	
};


/*
 * MountShortFileName:
 *     This function parses a directory entry name which is in the form 
 * of "FILE   EXT" and puts it in Buffer in the form of "FILE.TXT".
 * @todo fsMountShortFileName(...)
 */
void MountShortFileName(char *buffer, struct dir_entry_d *entry)
{
    int i = 0;
    
    // Get the file name.
    while(i < 8)
    {
        if( entry->FileName[i] == ' '){
            break;
        }

        buffer[i] = entry->FileName[i];
        ++i;
    };
 
    // Get extension.
    if( (entry->FileName[8] != ' ') )
    {
        buffer[i++] = '.';
        buffer[i++] = (entry->FileName[ 8] == ' ') ? '\0' : entry->FileName[ 8];
        buffer[i++] = (entry->FileName[ 9] == ' ') ? '\0' : entry->FileName[ 9];
        buffer[i++] = (entry->FileName[10] == ' ') ? '\0' : entry->FileName[10];
    };

done:	
    return;
};




/*
 * set_spc:
 *     Configura spc, 'Sector Per Cluster' em vari�vel global.
 */
void set_spc(int spc){
    g_spc = (int) spc;
    return;
};


/*
 * get_spc:
 *     Pega spc, Sector Per Cluster.
 */
int get_spc(){
    return (int) g_spc;
};


/*
 * get_filesystem_type:
 *     Pega o tipo de sistema de arquivos.
 */
int get_filesystem_type(){
    return (int) g_filesystem_type;
};


/*
 * set_filesystem_type:
 *     Configura o tipo de sistema de arquivo.
 */
void set_filesystem_type(int type)
{
    g_filesystem_type = (int) type;
    return;
};



/*
 * fs_init_fat:
 *     Inicializa a estrutura usada no sistema de arquivos.
 *     fsInitFat()
 */
void fs_init_fat()
{
	// File system structure.
	if( (void*) filesystem == NULL ){
	    printf("fs_init_fat error: fs.\n");
	    return;
	};

	// FAT structure.
	fat = (void*) malloc( sizeof(struct fat_d));
	if((void*) fat == NULL ){
	    printf("fs_init_fat error: fat.\n");
	    return;
	};
	
	// Info.
	fat->address = filesystem->fat_address; 
	fat->type = filesystem->type;
	//Continua ...
	
	
	//
	// Continua a inicializa��o da fat.
	//
	
done:
	return;
};	
	

/*
 * fs_init_structures:
 *     Inicializa a estrutura do sistema de arquivos.
 *     fsInitStructures
 */
void fs_init_structures()
{
    int Type;
	
	// Structure.
    filesystem = (void*) malloc( sizeof(struct filesystem_d));
	if( (void*) filesystem == NULL){
	    printf("fs_init_structures:");
        die();
	};
	
	//Type.
	Type = (int) get_filesystem_type();    //vari�vel.	
	if( Type == 0 ){
	    printf("fs_init_structures error: Type.");
        die();
	};	
	
	filesystem->type = (int) Type;
	
    switch(Type)
	{
	    case FS_TYPE_FAT16:
	        
			//Rootdir.
			filesystem->rootdir_address = FAT16_ROOTDIR_ADDRESS;
	        filesystem->rootdir_lba = FAT16_ROOTDIR_LBA;
	        
			//Fat.
			filesystem->fat_address = FAT16_FAT_ADDRESS;
	        filesystem->fat_lba = FAT16_FAT_LBA;
	        
			//Dataarea.
			//filesystem->dataarea_address = ??;
	        filesystem->dataarea_lba = FAT16_DATAAREA_LBA;
	        
			//sectors per cluster.
			filesystem->spc = (int) get_spc(); //variavel
	        filesystem->rootdir_entries = FAT16_ROOT_ENTRIES;
	        filesystem->entry_size = FAT16_ENTRY_SIZE;
		    break;
			
	    case FS_TYPE_EXT2:
		    //nothing for now.
		break;
		
		//case FS_TYPE_EXT3: break;
		
        default:
		    //nothing for now.
            break;		
	};

done:	
	return;
};


/*
 *************************************************************
 * fsInit:
 *     Inicializa o file system manager.
 *
 */
int fsInit()
{
	
#ifdef KERNEL_VERBOSE
    printf("fsInit: Initializing FS support..\n");
#endif 
 
	//
	// Type - Configura o tipo de sistema de arquivos usado. 
	// No caso, (fat16).
	//
	// @todo: Deve-se checar o volume ativo e ver qual sistema de arquivos est�
	//        sendo usado, ent�o depois definir configurar o tipo.
	//        O sistema operacional pode salvar o tipo usado. Nesse caso 
	//        apenas checar se ouve altera��es nas configura��es de sistema de arquivos.
	//        O registro de configura��es de disco pode ser armazenado em arquivos de metadados.
	//
	
	set_filesystem_type(FS_TYPE_FAT16);
	
	
	//
	// SPC ~ Configura o n�mero de setores por cluster.
	//       Nesse caso, s�o(512 bytes por setor, um setor por cluster).
    //
	
	set_spc(1);

	// Structures and fat.
    
#ifdef KERNEL_VERBOSE	
	printf("fsInit: Structures..\n");
#endif
	
	fs_init_structures();

#ifdef KERNEL_VERBOSE	
	printf("fsInit: FAT..\n");
#endif
	
	fs_init_fat();
	
	
	
	//
	// VFS
	//
	
#ifdef KERNEL_VERBOSE	
	printf("fsInit: VFS..\n");
#endif
	
	vfsInit();
	
	
	
	//selecionando disco, volume e diret�rio.
	//estamos resetando tudo e selecionando o diret�rio raiz 
	//do vfs ... mas na verdade o diret�rio selecionado 
	//deveria ser o diret�rio onde ficam a maioria dos aplicativos.
	//para que o usu�rio possa chamar o maior n�mero de apps usando 
	//apenas comandos simples.
    //#bugbug: isso deveria se passado pelo boot ??	
	current_disk = 0;
	current_volume = 0;
	current_directory = 0;
	
	//
	// @todo: Continua ...
	//
	
done:

#ifdef KERNEL_VERBOSE
    printf("Done!\n");
#endif 

    return (int) 0;    	
};


//
// End.
//

