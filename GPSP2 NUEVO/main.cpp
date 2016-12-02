#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include "fstream"
using namespace std;
struct ciudad*buscarCi(string );
void llenarCiudad(string,int,int);
void costruirCamino(string,string,int);
struct ciudad
{
    string nombre;
    sf::Vector2i cordenada;
    bool visitado;
    struct ciudad*vesino;
    struct calle*camino;
    ciudad(string n,int x,int y)
    {
        nombre = n;
        cordenada = sf::Vector2i(x,y);
        visitado = false;
        vesino = NULL;
        camino = NULL;
    }
}*mundo;
struct calle
{
    int distancia;
    struct ciudad*destino;
    struct calle*otraVia;
    calle(int d)
    {
        distancia = d;
        destino = NULL;
        otraVia = NULL;
    }
};
struct cola
{
    string ciu;
    int coste;
    struct cola*sigC;
    cola ()
    {
        ciu="";
        coste=0;
        cola*sigC=NULL;
    }
}*colaPri,*colaAux;

struct rCDis //Ausiliar para guardar en harchivo
{
    string ori;
    string des;
    int pesoT;
} gDis;
struct rCDis2 //Ausiliar para guardar en harchivo
{
    string ori;
    int x;
    int y;
} gDis2;
fstream fp1("\GPS_P2\Ciudades.txt", ios::in | ios::out|ios::binary);//Archivo que guarda las ciudades _ paises
fstream fp2("\GPS_P2\Caminos.txt", ios::in | ios::out|ios::binary);//Guarda las relaciones entre las ciudades _ paises
struct ciudad*buscarCi(string x)
{
    struct ciudad *tempCI = mundo;
    while(tempCI!= NULL)
    {

        if(tempCI->nombre == x)
            return tempCI;//retorna el nodo encontrado

        tempCI = tempCI->vesino;
    }
    return NULL; //no lo encontró
}
void llenarCiudad(string cid,int x,int y)
{
    struct ciudad * nnc = new ciudad (cid,x,y);
    gDis2.ori=cid;
    gDis2.x=x;
    gDis2.y=y;
    fp1.write(reinterpret_cast<char *> (&gDis2), sizeof(gDis2));
    nnc->vesino = mundo;//inserción al inicio
    mundo = nnc;
}
void costruirCamino(string orig,string dest,int v)
{
    struct ciudad * O = buscarCi(orig);
    struct ciudad * D = buscarCi(dest);
    if((O == NULL) or (D== NULL))
    {
        cout<<"\n Datos incorrectos no se encuentra el origen o destino.";
        return;
    }
    gDis.ori = orig;
    gDis.des = dest;
    gDis.pesoT = v;
    fp2.write(reinterpret_cast<char *> (&gDis), sizeof(gDis));
    struct calle *nna = new calle(v);
    nna->otraVia = O->camino;
    nna->destino=D;
    O->camino = nna;
}

void sacarCiudad()
{
    fstream temp("\GPS_P2\Ciudades.txt", ios::in | ios::out |ios::binary);
    temp.seekg(0);
    while(temp.eof() != true)
    {
        temp.read(reinterpret_cast<char *> (&gDis2), sizeof(gDis2));
        if(!temp.eof())
            llenarCiudad(gDis2.ori,gDis2.x,gDis2.y);
    }
    temp.close();
}
void sacarCaminos()
{
    fstream temp("\GPS_P2\Caminos.txt", ios::in | ios::out |ios::binary);
    temp.seekg(0);
    while(!temp.eof())
    {

        temp.read(reinterpret_cast<char *> (&gDis), sizeof(gDis));
        if(temp.eof() != true)
            costruirCamino(gDis.ori,gDis.ori,gDis.pesoT);
    }
    temp.close();
}
void datosPrueva()
{
    llenarCiudad("CostaRica",0,0);
    llenarCiudad("Nicaragua",0,0);
    llenarCiudad("Panama",0,0);
    llenarCiudad("USA",0,0);
    llenarCiudad("Mexico",0,0);
    llenarCiudad("Guatemala",0,0);
    llenarCiudad("Salvador",0,0);
    llenarCiudad("Cuba",0,0);
    llenarCiudad("Canada",0,0);
    llenarCiudad("Argentina",0,0);

    costruirCamino("CostaRica","Nicaragua",50);
    costruirCamino("CostaRica","Panama",80);
    costruirCamino("Nicaragua","CostaRica",50);
    costruirCamino("Nicaragua","Guatemala",100);
    costruirCamino("Nicaragua","Salvador",80);
    costruirCamino("Panama","CostaRica",80);
    costruirCamino("Panama","Argentina",150);
    costruirCamino("Salvador","Mexico",60);
    costruirCamino("Salvador","Nicaragua",80);
    costruirCamino("Salvador","Guatemala",50);
    costruirCamino("Argentina","Panama",150);
    costruirCamino("Guatemala","Nicaragua",100);
    costruirCamino("Guatemala","Mexico",90);
    costruirCamino("Guatemala","Salvador",50);
    costruirCamino("Canada","USA",200);
    costruirCamino("Mexico","USA",170);
    costruirCamino("Mexico","Guatemala",90);
    costruirCamino("Mexico","Salvador",40);
    costruirCamino("USA","Mexico",170);
    costruirCamino("USA","Canada",200);
    costruirCamino("USA","Cuba",110);
    costruirCamino("Cuba","USA",110);
}
bool existeRuta = false;
void hayRuta(string origen, string destino)
{
    struct ciudad *o = buscarCi(origen);

    if((o == NULL) or (o->visitado == true))
    {
        return;
    }
    if(origen == destino)
    {
        existeRuta = true;
        return;
    }
    o->visitado = true;
    struct calle*tempA = o->camino;
    while(tempA != NULL)
    {
        hayRuta(tempA->destino->nombre, destino);
        tempA = tempA->otraVia;
    }
}
//cuenta las rutas que existen del origen al destino.
int contRutas = 0 ;
void contarRutas (string origen, string destino)
{
    struct ciudad *o = buscarCi(origen);

    if((o == NULL) or (o->visitado == true))
        return;

    if(origen == destino)
    {
        contRutas++;
        return;
    }
    o->visitado = true;
    struct calle *tempA = o->camino;
    while(tempA != NULL)
    {
        contarRutas(tempA->destino->nombre, destino);
        tempA = tempA->otraVia;
    }
    o->visitado = false;
}
void desmarcar()
{
    struct ciudad *tempV = mundo;
    while(tempV!= NULL)
    {
        tempV->visitado = false;
        tempV = tempV->vesino;
    }
}
int peso,pesoTemp,pesoMe,gif;
void dijkstra(string origen,string destino)
{
    for(int i=0; i<contRutas; i++)
    {
        struct ciudad *o = buscarCi(origen);
        struct cola*temPC = new cola();
        if((o == NULL) or (o->visitado == true))
            return;
        if((origen == destino))
        {
            pesoMe;
            gif = pesoMe;
            return;
        }

        o->visitado = true;
        struct calle*tempA = o->camino;
        struct calle*tempB = o->camino;
        while(tempA != NULL)
        {

            temPC->coste =tempA->distancia;
            peso = tempA->distancia;
            temPC->ciu = tempA->destino->nombre;
            dijkstra(tempA->destino->nombre, destino);
            tempA = tempA->otraVia;

        }
        temPC->sigC = colaPri;
        colaPri = temPC;
        if(pesoTemp == 0)
        {
            pesoTemp = peso;
            pesoMe = peso;
            colaAux = colaPri;
        }
        else if(peso < pesoTemp)
        {
            pesoTemp = peso;
            pesoMe += peso;
            colaAux = colaPri;
        }
        o->visitado =false;
    }
}
void validarGrafo()
{
    if(mundo==NULL)
    {
        sacarCiudad();
        sacarCaminos();
    }
}
int main()
{
    desmarcar();
    datosPrueva();
    contarRutas("CostaRica","Mexico");
    dijkstra   ("CostaRica","Mexico");
    struct cola* nodo = colaAux;
    do{
        std::cout<<"\n"<<nodo->ciu;
        nodo=nodo->sigC;
    }while(nodo!=NULL);
    return 0;
}
