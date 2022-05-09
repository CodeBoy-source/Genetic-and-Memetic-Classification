/**
 * @file mytools.cpp
 * @version 2.5
 * @date 05/04/2022
 * @author Brian Sena Simons 3ºA-A2
 * @brief Herramientas definidas para la práctica.
 * @code
 * [...]
    RowVectorXd Weights = VectorXd::Constant(mat.cols(),1);
    min = minEuclideanDistance(Weights,mat.row(0), mat.block(row,0,upper_row - row,mat.cols()),index);
    cout << "Prueba de obtención de clase concreta" << endl;
    cout << "Obteniendo filas tipo g " << endl;
    MatrixXd typeG = getClass(mat,label,'g');
    cout << "Filas: " << typeG.rows() << " columnas: " << typeG.cols()  << endl;
    cout << "###################################" << endl;
 *[...]
 * @endcode
 **/
#include "../tools/mytools.h"
#include "../inc/eigen-3.4.0/Eigen/Dense"
#include "../inc/random.hpp"
#include <fstream>
#include <math.h>
#include <iostream>

using namespace std;
using namespace Eigen;
using namespace std::chrono;
using Random = effolkronium::random_static;

/**
 * En esta función hacemos un simple "for-loop" hasta upperlimit
 * dónde en cada iteración asignamos el valor "i" al vector
 * generando una sucesión de 0-upperlimit.
 */
void fillRange(vector<int>& toFill,unsigned int upperlimit){
    toFill.clear();
    for(unsigned int i=0;i<upperlimit;i++)
        toFill.push_back(i);
}

/**
 * La idea es un vector de índices a permutar. Dónde el valor de la posición
 * 0 es el índice a permutar con la posición 0. Si vector.at(0) = 2, intercambiamos
 * 0 con el 2.
 */
void shuffleData(MatrixXd& mat,vector<char>& label,long int seed){
    Random::seed(seed);
    vector<int> indexes;
    fillRange(indexes,mat.rows());
    Random::shuffle(indexes);
    Transpositions<Dynamic, Dynamic> tr;
    tr.resize(mat.rows());

    for(unsigned int i=0;i<indexes.size();i++){
        iter_swap(label.begin()+i,label.begin()+indexes.at(i));
        tr[i] = indexes.at(i);
    }

    MatrixXd temp = tr * mat;
    mat = temp;
}

void shuffleFit(MatrixXd& mat,RowVectorXd& fitness, long int seed){
    if(seed!=-1)
        Random::seed(seed);
    vector<int> indexes;
    fillRange(indexes,mat.rows());
    Random::shuffle(indexes);
    Transpositions<Dynamic, Dynamic> tr;
    tr.resize(mat.rows());
    float var = -1;

    for(unsigned int i=0;i<indexes.size();i++){
        var = fitness(indexes.at(i));
        fitness(indexes.at(i)) = fitness(i);
        fitness(i) = var ;
        tr[i] = indexes.at(i);
    }

    MatrixXd temp = tr * mat;
    mat = temp;
}


void shuffleFit(MatrixXd& mat, MatrixXd& GenData, long int seed){
    if(seed!=-1)
        Random::seed(seed);
    vector<int> indexes;
    fillRange(indexes,mat.rows());
    Random::shuffle(indexes);
    Transpositions<Dynamic, Dynamic> tr;
    tr.resize(mat.rows());

    for(unsigned int i=0;i<indexes.size();i++){
        tr[i] = indexes.at(i);
    }

    MatrixXd temp = tr * mat;
    mat = temp;
    temp = tr * GenData;
    GenData = temp;
}

void shuffleOnly(MatrixXd& mat, long int seed){
    if(seed!=-1)
        Random::seed(seed);
    vector<int> indexes;
    fillRange(indexes,mat.rows());
    Random::shuffle(indexes);
    Transpositions<Dynamic, Dynamic> tr;
    tr.resize(mat.rows());

    for(unsigned int i=0;i<indexes.size();i++){
        tr[i] = indexes.at(i);
    }

    MatrixXd temp = tr * mat;
    mat = temp;
}

/**
 * La función sobreescribe la fila con la matrix que hay después de esa fila.
 * Y Luego hace un resize para cambiar el tamaño original a una fila menos.
 **/
Eigen::MatrixXd removeRow(Eigen::MatrixXd matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);

    matrix.conservativeResize(numRows,numCols);
    return matrix;
}

/**
 * La función sobreescribe la columna con los valores que hay después de esa columna.
 * Y Luego hace un resize para cambiar el tamaño original a una columna menos.
 **/
Eigen::MatrixXd removeColumn(Eigen::MatrixXd matrix, unsigned int colToRemove)
{
    unsigned int numRows = matrix.rows();
    unsigned int numCols = matrix.cols()-1;

    if( colToRemove < numCols )
        matrix.block(0,colToRemove,numRows,numCols-colToRemove) = matrix.block(0,colToRemove+1,numRows,numCols-colToRemove);

    matrix.conservativeResize(numRows,numCols);
    return matrix;
}

/*
 * @brief Iteramos sobre el vector de puntuación y devolvemos un vector de
 * tamaño size con los índices de los "size" primeros mejores valores.
 * @param Fitness Vector a iterar.
 * @param indexGrid Índices resultantes.
 * @param size Número de posiciones a computar.
 */
void getBest(RowVectorXd Fitness,vector<int>& indexGrid,unsigned int size){
    indexGrid.clear();
    vector<int> values, sorted;
    for(unsigned int i=0;i<Fitness.cols();i++){
        values.push_back(Fitness(i));
        sorted.push_back(Fitness(i));
    }
    sort(sorted.begin(),sorted.end());
    vector<int>::iterator index;
    size = (size>Fitness.cols())? Fitness.cols():size;
    for(unsigned int i=0;i<size;++i){
        index = find(values.begin(),values.end(),sorted[i]);
        indexGrid.push_back(int(index-values.begin()));
    }
}
