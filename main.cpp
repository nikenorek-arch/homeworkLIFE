#include<iostream>
#include<fstream>
#include<windows.h>
#include<cstdlib>
#include<unistd.h>

using namespace std;


// создает двумерный массив
int** create_two_dim_array(int row, int col){
    int** arr = new int *[row];
    for (int i = 0; i < row; i++){
        arr[i] = new int [col];
    }
    return arr;
}

//заполняет нулями двумерный массив
void fill_two_dim_array(int** arr, int row, int col){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            arr[i][j] = 0;
        }
    }
}

//копирует значения из одного первого двумерного массива во второй
void copy_two_dim_array(int** arr1, int** arr2, int row, int col){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            arr2[i][j] = arr1[i][j];
        }
    }
}


//сравнивает равны ли два двумерных массива
bool compare_two_dim_array(int** arr1, int** arr2, int row, int col){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            if (arr2[i][j] != arr1[i][j]) return false;
        }
    }

    return true;
}

//считает количество живих соседей у клетки массива
int getCountNighbour_two_dim_array(int** arr, int i_, int j_, int row, int col){

    int count = 0;

    for(int i = i_-1; i <= i_+1; i++){
        for(int j = j_-1; j <= j_+1; j++){

            if (i < 0 || j < 0) continue;
            if (i >= row || j >= col) continue;
            if (i == i_ && j == j_) continue;

            if (arr[i][j] == 1) count++;
        }
    }

    return count;
}

//в целом по массиву считает количество живых клеток
int getCountLife_two_dim_array(int** arr, int row, int col){

    int count = 0;

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            if (arr[i][j] == 1) count++;
        }
    }

    return count;
}


//обновление основного массива
void newLife_two_dim_array(int** arr, int** arrCopy, int row, int col){
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            int count = getCountNighbour_two_dim_array(arrCopy, i, j, row, col);
            if (count == 3 && arrCopy[i][j] == 0) arr[i][j] = 1;
            else if ((count == 2 || count == 3) && arrCopy[i][j] == 1) arr[i][j] = 1;
            else if ((count < 2 || count > 3) && arrCopy[i][j] == 1) arr[i][j] = 0;
        }
    }
}

//вывод массива на экран
void print_two_dim_array(int** arr, int row, int col){

    system("cls");

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            char sim = '-';
            if (arr[i][j] == 1) sim = '*'; 
            cout << sim << " ";
        }
        cout << endl;
    }
}

//удаление массива из памяти
void delete_two_dim_array(int** arr, int row, int col){
    for (int i = 0; i < row; i++) delete[] arr[i];
    delete[] arr;
}


//чтение файла
void addLifeInArrayFromFile(int& row, int& col, int**& myField, int**& myFieldCopy, bool& read){
    int k = 0;
    int r = 0, c = 0;

    fstream file("in.txt", ios::in);
    if (file.is_open()){
        while(!file.eof()){

            if (k == 0){
                file >> row;
            }
            else if (k == 1){
                file >> col;

                //создаю массивы по размеру из txt
                myField = create_two_dim_array(row, col);
                myFieldCopy = create_two_dim_array(row, col);
                fill_two_dim_array(myField, row, col);

            }
            else{
                if(k%2 == 0) {
                    file >> r;
                }
                else{
                    //заполняю живые клетки по координатам из txt
                    file >> c;
                    myField[r][c] = 1;
                }
            }
            k++;
        }
    }
    else{
        cout << "Файл Files/in1.txt не найден!";
        read = true;
    }

}



int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    int row = 0;
    int col = 0;
    bool read = false;

    int** myField;
    int** myFieldCopy;

    addLifeInArrayFromFile(row, col, myField, myFieldCopy, read);

    if (read) {return 0;}                                               //если файл не был прочитан, прервем программу

    copy_two_dim_array(myField, myFieldCopy, row, col);

    bool arrEqual = false;
    int lifeCount = 0;
    int genCount = 0;

    do{
        sleep(1);
        genCount++;


        newLife_two_dim_array(myField, myFieldCopy, row, col);
        
        arrEqual = compare_two_dim_array(myField, myFieldCopy, row, col);
        lifeCount = getCountLife_two_dim_array(myField, row, col);

        copy_two_dim_array(myField, myFieldCopy, row, col);
        print_two_dim_array(myField, row, col);
        cout << "Generation: " << genCount << ". Alive cells: " << lifeCount << endl;

    }
    while(!arrEqual && lifeCount > 0);

    if (arrEqual) cout << "The world has stagnated. Game over";
    if (lifeCount == 0) cout << "All cells are dead. Game over";


    delete_two_dim_array(myField, row, col);
    delete_two_dim_array(myFieldCopy, row, col);


}