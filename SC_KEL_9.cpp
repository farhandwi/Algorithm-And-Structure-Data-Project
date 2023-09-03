#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#define SPACE 10
#define limit 100
using namespace std;

struct Tree{
    string jenis_Kendaraan, plat_kendaraan;
    int antrian, id_Tree;
    Tree *right, *left;
    Tree(string a, string plat, int id){
        jenis_Kendaraan = a;
        plat_kendaraan = plat;
        id_Tree = id;
        right = left = NULL;
    }
    Tree(){
        right = left = NULL;
    }
};

Tree *Root;

struct Queue{
    int antrian;
    string jenis_Kendaraan;
    Queue *next;
    Queue *prev;
};
Queue *Head, *Tail;

int queue = 0;
string jumlah = "1";
string jenis_Kendaraan[limit], nomor_Plat[limit], nomor_Antrian[limit], ID[limit], IDTREE[limit];
char isLanjut;
string Temporary;

bool login();
void menu_login();
void checkDatabase(fstream &storage);
void display_Antrian(fstream &storage);
void enqueue();
void dequeue();
bool is_Empty();
void display_Queue();
void addAntrian(fstream &storage);
void jumlah_Antrian(fstream &storage, bool cek);
void display2D(Tree *root, int space);
void Postorder(struct Tree* node);
void inorder(struct Tree *node);
void Preorder(struct Tree *node);
void deleteAntrian(fstream &storage);
void cariKendaraan(fstream &storage);
Tree *iterativeSearch(int v);
Tree *deleteNode(Tree *r, int v);
Tree *minValueNode(Tree *node);
Tree *insert(Tree *node, string plat, string data_kendaraan, int &id, int &tingkat);
Tree *newTree(string data_kendaraan, string plat, int id);

int main(int argc, char const *argv[]){
    char is_continue;
    menu_login();
    int inputan;
    bool cek = false;
    string cari_kendaraan[limit], temp[limit];
    fstream storage;
    checkDatabase(storage);
    int j = 0, i = 0;
    while(!storage.eof()){
        getline(storage, cari_kendaraan[i], '\t');
        getline(storage, temp[j+11], '\n');
        i++;
    }
    if(i >= 0 && cari_kendaraan[0] != ""){
        cek = true;
    }
    jumlah_Antrian(storage, cek);
    storage.close();
    char loop;
    menu:
    system("cls");
    display_Queue();
    cout << "============PERTAMINA UNIVERSITY PARKING SYSTEM PROGRAM=============" << endl;
    cout << "1. Enqueue A New Parking Queue\n2. Dequeue Parking Queue To Parking Lot\n3. Display List of Vehicles Entering the Parking Lot\n4. Display Tree Parking Lot\n5. Search Vehicle\n6. Get Out of the Parking Lot\n7. Exit" << endl;
    cout << "====================================================================\n Enter your choice: ";
    cin >> inputan;
    cin.ignore();
    enum option{ENQUEUE = 1, DEQUEUE, DISPLAY_QUEUE, DISPLAY_TREE, SEARCH, DELETE, FINISH};
    while(inputan != FINISH){
        switch (inputan){
        case ENQUEUE: //nambahin barang store ke Antrian.txt
        {
            enqueue();
            cout << "Queue Has Been Added" << endl;
            system("PAUSE");
            goto menu;
        }
        case DEQUEUE: //nampilin yang udh di store ke Antrian.txt nya
        {
            addAntrian(storage);
            dequeue();
            system("PAUSE");
            goto menu;
        }
        case DISPLAY_QUEUE:
        {
            cout << "=======Display List of Vehicles Entering the Parking Lot=======" << endl;
            display_Antrian(storage);
            system("PAUSE");
            goto menu;
        }
        case DISPLAY_TREE:
        {
            cout << "=====================DISPLAY TREE========================\n" << endl;
            display2D(Root, 10);
            cout << endl;
            if(Root == NULL){
                cout << "Data Tree is Still Empty, fill it in first!" << endl;
            }else{
                cout << "Traversal Preorder: ";
                Preorder(Root);
                cout << endl;
                cout << "Traversal InOrder: ";
                inorder(Root);
                cout << endl;
                cout << "Traversal PostOrder: ";
                Postorder(Root);
                cout << endl;
            }
            system("PAUSE");
            goto menu;
            break;
        }
        case SEARCH:
        {
            display_Antrian(storage);
            cariKendaraan(storage);
            system("PAUSE");
            goto menu;
        }
        case DELETE:
        {
            deleteAntrian(storage);
            system("PAUSE");
            goto menu;
        }
        case FINISH:
            exit(0);
        default:
        {
            cout << "Choose 1-7" << endl;
            break;
        }
        }
    
    lanjutkan:
	cout << "Continue?(y/n) : ";
	cin >> is_continue;
    cin.ignore();
	if ( (is_continue == 'y') | (is_continue == 'Y')){
        goto menu;
    } else if ((is_continue == 'n') | (is_continue == 'N')){
		break;
	} else {
		goto lanjutkan;
	}
    }
    
    return 0;
}

bool login(){
    fstream myFile;
    myFile.open("data.txt", ios::in);
    string username, password, simpan_username, simpan_password;
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
  
    while(!myFile.eof()){
      getline(myFile, simpan_username);
      getline(myFile, simpan_password);
    }
    if (simpan_username == username && simpan_password == password){
        return true;
    }else{
        return false;
    }
    myFile.close();
}

void menu_login(){
    string username, password;
    int pilihan;
    fstream myFile;
    myFile.open("data.txt",ios::out|ios::in);
    label:
    system("cls");
    cout << "===========LOGIN PARKIR LOT UNIVERSITAS PERTAMINA===========" << endl;
    cout << "1. Register a new account\n2. Login\n Enter your choice (enter 0 if you want to exit): ";
    cin >> pilihan;
    cin.ignore();
    if (pilihan == 1){
        if(!myFile){
            myFile.close();
            myFile.open("data.txt",ios::out);
            cout << "Choose your username: ";
            getline(cin, username);
            cout << "Choose your password: ";
            getline(cin, password);
            myFile << username << endl << password;
            myFile.close();
            cout << "Yoo! New account has been created ^-^" << endl;
            system("PAUSE");
            goto label;
        }else{
            cout << "Sorry! account has been created" << endl;
            system("PAUSE");
            goto label;
        }

    }else if(pilihan == 2){
        if(!myFile){
            cout << "Sorry! Account not yet created, please register an account!" << endl;
            system("PAUSE");
            goto label;
        }
        bool status = login();
        if (status == true){
            cout << "Login successful! ^-^" << endl;
            system("PAUSE");
        }else{
            cout << "Login Unsuccessful! enter username and password correctly!" << endl;
            system("PAUSE");
            goto label;
        }
    }else if (pilihan == 0){
      exit(0);
    }else{
        cout << "Your input is wrong!" << endl;
        system("PAUSE");
        goto label;
    }
}

void checkDatabase(fstream &storage){
	storage.open("Antrian.txt", ios::out | ios::in);
	// check file ada atau tidak
    if (storage.is_open()){
        cout << "database ditemukan" << endl;
        storage << "Nomor Antrian\t\t";
        storage << "Nomor Plat\t\t";
        storage << "ID Tinkat\t\t";
        storage << "ID Tree\t\t";
        storage << "Jenis Kendaraan" << endl << endl;;
    } else {
		//cout << "database tidak ditemukan, buat database baru" << endl;
		storage.close();
		storage.open("Antrian.txt", ios::trunc | ios::out | ios::in);
        storage << "Nomor Antrian\t\t";
        storage << "Nomor Plat\t\t";
        storage << "ID Tingkat\t\t";
        storage << "ID Tree\t\t";
        storage << "Jenis Kendaraan" << endl << endl;
    }
}

void jumlah_Antrian(fstream &storage, bool cek){
    if(cek == true){
        string temp[limit];
        string jumlah_awal[limit], nomor[limit], id_tingkat[limit], id_tree[limit], jenis[limit];
        int j = 0;
        int i = 0;
        char sampah[limit], sam[limit];
        storage.close();
        storage.open("Antrian.txt", ios::in);
        while(!storage.eof()){
            while(j < 2){
                getline(storage, temp[j]);
                j++;
            }
            getline(storage, jumlah_awal[i], '\t');
            getline(storage, temp[j], '\t');
            getline(storage, temp[j+1], '\t');
            getline(storage, nomor[i], '\t');
            getline(storage, temp[j+2], '\t');
            getline(storage, id_tingkat[i], '\t');
            getline(storage, temp[j+3], '\t');
            getline(storage, temp[j+4], '\t');
            getline(storage, id_tree[i], '\t');
            getline(storage, temp[j+5], '\t');
            getline(storage, temp[j+6], '\t');
            getline(storage, jenis[i], '\n');
            i++;
            j++;
        }
        for (int k = 0; k < i-1; k++)
        {
            strcpy(sampah, id_tree[k].c_str());
            int id = atoi(sampah);
            char sam[100];
            strcpy(sam, id_tree[k].c_str());
            int tingkat = atoi(sam);
            Root = insert(Root, nomor[k], jenis[k], id, tingkat);
        }
        
        char arr[100];
        Temporary = jumlah_awal[i-2];
        strcpy(arr, Temporary.c_str());
        int sementara = atoi(arr);
        sementara++;
        jumlah = to_string(sementara);
    }else{
        return;
    }
}

void enqueue(){
    Queue *Temp = new Queue;
    int temp = atoi(jumlah.c_str());
    Temp->antrian = temp;
    Temp->jenis_Kendaraan = "";
    if(is_Empty()){
        Temp->next = NULL;
        Temp->prev = NULL;
        Head = Temp;
        Tail = Temp;
        temp++;
        string ToString = to_string(temp);
        jumlah = ToString;
        queue++;
    }else if(queue >= limit){
        cout << "Queue Full, Please wait!" << endl;
    }else{
        Temp->next = NULL;
        Temp->prev = Tail;
        Tail->next = Temp;
        Tail = Temp;
        temp++;
        string ToString = to_string(temp);
        jumlah = ToString;
        queue++;
    }
}

void dequeue(){
    Queue *Hapus, *Temp;
    Hapus = Head;
    if(is_Empty()){
        cout << "The queue is still empty!" << endl;
    }else if(Head->next == NULL){
        Hapus->next = NULL;
        Hapus->prev = NULL;
        Hapus = NULL;
        Head = Hapus;
        Tail = Head;
        queue--;
    }else{
        Temp = Hapus->next;
        Hapus->next = NULL;
        Hapus->prev = NULL;
        Head = Temp;
        Head->prev = NULL;
        delete Hapus;
        queue--;
    }
}

void display_Antrian(fstream &storage){ 
    char loop;
    string word;
    storage.open("Antrian.txt", ios::in);
    cout << "==================================================================================================" << endl;
    while(!storage.eof()){
        getline(storage, word, '\t');
        cout << word << "\t";
    }
    cout << endl;
    cout << "==================================================================================================" << endl;
    storage.close();
}

bool is_Empty(){
    if(Head == NULL){
        return 1;
    }else{
        return 0;
    }
}

void display_Queue(){
    Queue *Temp;
    Temp = Head;
    if(is_Empty()){
        cout << "Empty Queue!" << endl;
    }else{
        cout << "           Queue        " << endl;
        cout << "===============================\n" << endl;
        cout << "Filled Queue Number: [ ";
        while(Temp != NULL){
            cout << Temp->antrian << " ";
            Temp = Temp->next;
        }
        cout << "]" << endl;
        cout << "===============================\n" << endl;
    }
}

void addAntrian(fstream &storage){
    string temp[limit], nPlat[limit], jenis[limit], nAntrian[limit], id[limit], idTree[limit];
    int tingkat_Antrian;
    int posisi = 0;
    do{
        if(posisi < limit){
            if(!is_Empty()){
                int i = 0, j = 0;
                display_Antrian(storage);
                storage.open("Antrian.txt", ios::in);
                while(!storage.eof()){
                    while(j < 2){
                        getline(storage, temp[j]);
                        j++;
                    }
                    getline(storage, nAntrian[i], '\t');
                    getline(storage, temp[j], '\t');
                    getline(storage, temp[j+1], '\t');
                    getline(storage, nPlat[i], '\t');
                    getline(storage, temp[j+2], '\t');
                    getline(storage, id[i], '\t');
                    getline(storage, temp[j+3], '\t');
                    getline(storage, temp[j+4], '\t');
                    getline(storage, idTree[i], '\t');
                    getline(storage, temp[j+5], '\t');
                    getline(storage, temp[j+6], '\t');
                    getline(storage, jenis[i], '\n');
                    i++;
                }
                storage.close();
                storage.open("Antrian.txt", ios::out|ios::app);
                ID:
                cout << "Enter Vehicle Type: ";
                getline(cin, jenis_Kendaraan[posisi]);
                cout << "Enter Vehicle Plate Number: ";
                getline(cin, nomor_Plat[posisi]);
                cout << "Enter Vehicle Weight: ";
                getline(cin, IDTREE[posisi]);
                for (int v = 0; v < i - 1; v++){
                    if(nPlat[v] == nomor_Plat[posisi]){
                        cout << "Sorry! Number Plate Already Used!" << endl;
                        system("PAUSE");
                        goto ID;
                    }else{
                        continue;
                    }
                }
                char sw[100];
                strcpy(sw, IDTREE[posisi].c_str());
                int ID = atoi(sw);
                int tingkat = 0;
                Root = insert(Root, nomor_Plat[posisi], jenis_Kendaraan[posisi], ID, tingkat);
                string toString = to_string(Head->antrian);
                nomor_Antrian[posisi] = toString;
                storage << nomor_Antrian[posisi] << "\t\t\t";
                storage << nomor_Plat[posisi] << "\t\t";
                storage << tingkat << "\t\t\t";
                storage << IDTREE[posisi] << "\t\t\t";
                storage << jenis_Kendaraan[posisi] << "\n";
                posisi++;
                storage.close();
                system("cls");
                display_Antrian(storage);
                cout << "Queue Number" << Head->antrian << " Entering the Parking Lot" << endl;
                break;
            }else{
                cout << "No Queues!" << endl;
                break;
            }
        }else{
            cout << "Sorry! Memory full " << endl;
            system("PAUSE");
            break;
        }
    }while (true);
    storage.close();
}

struct Tree *newTree(string data_kendaraan, string plat, int id){
	Tree *temp = new Tree;
	temp->jenis_Kendaraan = data_kendaraan;
    temp->plat_kendaraan = plat;
    temp->id_Tree = id;
	temp->left = temp->right = NULL;
	return temp;
}

struct Tree *insert(Tree *node, string plat, string data_kendaraan, int &id, int &tingkatan){
	if (node == NULL){
		return newTree(data_kendaraan, plat, id);
    }

	if (id < node->id_Tree){
        tingkatan++;
		node->left = insert(node->left, plat, data_kendaraan, id, tingkatan);
	}else if (id >= node->id_Tree){
        tingkatan++;
		node->right = insert(node->right, plat, data_kendaraan, id, tingkatan);
	}
	return node;
}

void display2D(Tree *root, int space){
    if (root == NULL){
        return;
    }
    space += SPACE;
    display2D(root->right, space);
    cout << endl;
    for (int i = SPACE; i < space; i++){
        cout << " ";
    }
    cout << root->jenis_Kendaraan << endl; 
    display2D(root->left, space);
}

void inorder(struct Tree* node){
	if (node == NULL)
		return;

	/* first recur on left child */
	inorder(node->left);

	/* then print the data of node */
	cout << node->jenis_Kendaraan << " ";

	/* now recur on right child */
	inorder(node->right);
}

void Preorder(struct Tree* node){
    if (node == NULL)
        return;
 
    /* first print data of node */
    cout << node->jenis_Kendaraan << " ";
 
    /* then recur on left subtree */
    Preorder(node->left);
 
    /* now recur on right subtree */
    Preorder(node->right);
}

void Postorder(struct Tree* node){
    if (node == NULL)
        return;
 
    Postorder(node->left);
 
    Postorder(node->right);
 
    cout << node->jenis_Kendaraan << " ";
}

void cariKendaraan(fstream &storage){
    string temp[100], input_Jenis, input_Plat;
    bool found = false;
    int i = 0, j=0, h = 0, pil;
    string word;
    storage.open("Antrian.txt", ios::in|ios::out);
    while(!storage.eof()){
        while(j < 2){
            getline(storage, temp[j]);
            j++;
        }
        getline(storage, nomor_Antrian[i], '\t');
        getline(storage, temp[j], '\t');
        getline(storage, temp[j+1], '\t');
        getline(storage, nomor_Plat[i], '\t');
        getline(storage, temp[j+2], '\t');
        getline(storage, ID[i], '\t');
        getline(storage, temp[j+3],'\t');
        getline(storage, temp[j+4],'\t');
        getline(storage, IDTREE[i], '\t');
        getline(storage, temp[j+5],'\t');
        getline(storage, temp[j+6],'\t');
        getline(storage, jenis_Kendaraan[i], '\n');
        i++;
    }
    cout << "Enter the type of vehicle you want to search for: ";
    getline(cin, input_Jenis);
    cout << "Enter the number plate you want to search for: ";
    getline(cin, input_Plat);
    for (int q = 0; q < i-1; q++){
        if (input_Jenis == jenis_Kendaraan[q] && input_Plat == nomor_Plat[q]){
            cout << input_Jenis << " with number plate " << input_Plat << " found on the levels " << ID[q] << endl;
            found = true;
            break;
        }
    }
    if(found == false){
        cout << "Vehicle Not Found!" << endl;
    }
    storage.close();
}

void deleteAntrian(fstream &storage){
    char pilihan;
    bool found = false;
    display_Antrian(storage);
    string temp[limit], temporary[limit], input;
    int i = 0, j = 0, pil;
    string word;
    Tree *newNode = new Tree();
    storage.open("Antrian.txt", ios::in|ios::out);
    while(!storage.eof()){
        while(j < 2){
            getline(storage, temp[j]);
            j++;
        }
        getline(storage, nomor_Antrian[i], '\t');
        getline(storage, temp[j], '\t');
        getline(storage, temp[j+1], '\t');
        getline(storage, nomor_Plat[i], '\t');
        getline(storage, temp[j+2], '\t');
        getline(storage, ID[i], '\t');
        getline(storage, temp[j+3], '\t');
        getline(storage, temp[j+4], '\t');
        getline(storage, IDTREE[i], '\t');
        getline(storage, temp[j+5], '\t');
        getline(storage, temp[j+6], '\t');
        getline(storage, jenis_Kendaraan[i], '\n');
        i++;
    }
    ID:
    cout << "Masukkan plat nomor yang ingin dihapus: ";
    getline(cin, input);
    for (int v = 0; v < i-1; v++){
        if (input == nomor_Plat[v]){
            char to_int[100];
            strcpy(to_int, IDTREE[v].c_str());
            int id = atoi(to_int);
            found = true;
            newNode = iterativeSearch(id);
            if (newNode != NULL){
                cout << "Vehicle " << jenis_Kendaraan[v] << " has left the parking lot!" << endl;
                deleteNode(Root, id);
                cout << "Value Deleted" << endl;
            }else{
                cout << "Value NOT found" << endl;
            }
            temporary[v] = jenis_Kendaraan[v];
            temporary[v + 1] = nomor_Antrian[v];
            temporary[v + 2] = nomor_Plat[v];
            temporary[v + 3] = ID[v];
            temporary[v + 4] = IDTREE[v];
            for (v; v < i-2; v++){
                jenis_Kendaraan[v] = jenis_Kendaraan[v + 1];
                nomor_Antrian[v] = nomor_Antrian[v + 1];
                nomor_Plat[v] = nomor_Plat[v + 1];
                ID[v] = ID[v + 1];
                IDTREE[v] = IDTREE[v + 1];
            }
            storage.close();
            storage.open("Antrian.txt", ios::trunc|ios::out|ios::in);
            storage << "Nomor Antrian\t\t";
            storage << "Nomor Plat\t\t";
            storage << "ID Tinkat\t\t";
            storage << "ID Tree\t\t";
            storage << "Jenis Kendaraan" << endl << endl;
            for (int k = 0; k < i-2; k++){
                storage << nomor_Antrian[k] << "\t\t\t";
                storage << nomor_Plat[k] << "\t\t";
                storage << ID[k] << "\t\t\t";
                storage << IDTREE[k] << "\t\t\t";
                storage << jenis_Kendaraan[k] << "\n";
            }
            storage.close();
            display_Antrian(storage);
            return;
            break;
        }else{
            continue;
        }

    }

    if(found == false){
        cout << "Sorry! ID number not found" << endl;
        storage.close();
        storage.open("Antrian.txt", ios::trunc|ios::out|ios::in);
        storage << "Nomor Antrian\t\t";
        storage << "Nomor Plat\t\t";
        storage << "ID Tinkat\t\t";
        storage << "ID Tree\t\t";
        storage << "Jenis Kendaraan" << endl << endl;
        for (int k = 0; k < i-1; k++){
            storage << nomor_Antrian[k] << "\t\t\t";
            storage << nomor_Plat[k] << "\t\t";
            storage << ID[k] << "\t\t\t";
            storage << IDTREE[k] << "\t\t\t";
            storage << jenis_Kendaraan[k] << "\n";
        }
        storage.close();
        display_Antrian(storage);
    }
}

Tree *iterativeSearch(int v){
    if (Root == NULL){
        return Root;
    }else{
        Tree *Temp = Root;
        while (Temp != NULL){
        if(v == Temp->id_Tree){
            return Temp;
        }else if(v < Temp->id_Tree){
            Temp = Temp->left;
        }else{
            Temp = Temp->right;
        }
    }
      return NULL;
    }
}

Tree * minValueNode(Tree *node){
    Tree *current = node;
    /* loop down to find the leftmost leaf */
    while(current->left != NULL){
        current = current->left;
    }
    return current;
}

Tree *deleteNode(Tree *root, int v) {
    if (root == NULL) {
        return NULL;
    }
    else if (v < root->id_Tree) {
      root->left = deleteNode(root->left, v);
    }
    else if (v > root->id_Tree) {
      root->right = deleteNode(root->right, v);
    }
    else{
        if (root->left == NULL) {
            Tree *Temp = root->right;
            delete root;
            return Temp;
        }else if (root->right == NULL) {
            Tree *Temp = root->left;
            delete root;
            return Temp;
        }else{
            Tree *Temp = minValueNode(root->right);
            root->id_Tree = Temp->id_Tree;
            root->jenis_Kendaraan = Temp->jenis_Kendaraan;
            root->plat_kendaraan = Temp->plat_kendaraan;
            root->right = deleteNode(root->right, Temp->id_Tree);
        }
    }
    return root;
}