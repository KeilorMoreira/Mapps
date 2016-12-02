#include <SFML/Graphics.hpp>
#include <iostream>
#include "fstream"
//using namespace std;
//using namespace sf;




struct ciudad*buscarCi(std::string );
void llenarCiudad(std::string,int,int);
void costruirCamino(std::string,std::string,int);


struct ciudad
{
    std::string nombre;
    std::string cod;
    sf::Vector2f cordenada;
    bool visitado;
    struct ciudad*vesino;
    struct calle*camino;
    ciudad(std::string c,std::string n,int x,int y)
    {
        cod=c;
        nombre = n;
        cordenada = sf::Vector2f(x,y);
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

struct rCDis //Ausiliar para guardar en harchivo
{
    std::string ori;
    std::string des;
    int pesoT;
} gDis;

struct rCDis2 //Ausiliar para guardar en harchivo
{
    std::string cod;
    std::string ori;
    int x;
    int y;
} gDis2;

std::fstream fp1("\GPS_P2\Ciudades.txt", std::ios::in | std::ios::out|std::ios::binary);//Archivo que guarda las ciudades _ paises
std::fstream fp2("\GPS_P2\Caminos.txt", std::ios::in | std::ios::out|std::ios::binary);//Guarda las relaciones entre las ciudades _ paises
struct ciudad*buscarCi(std::string x)
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

struct ciudad*buscarCodCi(std::string cd) // Retorna el vector de una ciudad encontrada.
{
    struct ciudad *tempCI = mundo;
    while(tempCI!= NULL)
    {

        if(tempCI->cod == cd)
            return tempCI;//retorna del nodo encontrado

        tempCI = tempCI->vesino;
    }
    return NULL; //no lo encontró
}

struct Linea
{
    Linea()
    {
        m_Pos = sf::Vector2f(.0f, .0f);
        m_Size = sf::Vector2f(50.0f, 50.0f);
    }
    Linea(sf::Vector2f pos, sf::Vector2f size = sf::Vector2f(50.0f, 50.0f))
    {
        m_Pos = pos;
        m_Size = size;
    }

    sf::Vector2f m_Pos;
    sf::Vector2f m_Size;
};
std::vector<Linea> Ruta;

struct Bandera // Permite pintar banderas o marcas en varias partes del mapa, solo tiene vectores
{
    Bandera()
    {
        m_Pos = sf::Vector2f(.0f, .0f);
        m_Size = sf::Vector2f(50.0f, 50.0f);
    }
    Bandera(sf::Vector2f pos, sf::Vector2f size = sf::Vector2f(50.0f, 50.0f))
    {
        m_Pos = pos;
        m_Size = size;
    }

    sf::Vector2f m_Pos;
    sf::Vector2f m_Size;
};
std::vector<Bandera> Banderas;
void llenarCiudad(std::string cod,std::string cid,int x,int y)
{
    Bandera nueva = Bandera(sf::Vector2f(x,y),sf::Vector2f(6,6));// Agrega nueva bandera, posicion y medidas del dibujo.
    Banderas.push_back(nueva);
    struct ciudad * nnc = new ciudad (cod,cid,x,y);
    gDis2.cod=cod;
    gDis2.ori=cid;
    gDis2.x=x;
    gDis2.y=y;
    fp1.write(reinterpret_cast<char *> (&gDis2), sizeof(gDis2));
    nnc->vesino = mundo;//inserción al inicio
    mundo = nnc;
}


void costruirCamino(std::string orig,std::string dest,int v)
{
    struct ciudad * Or = buscarCi(orig);
    struct ciudad * De = buscarCi(dest);
    if((Or == NULL) or (De== NULL))
    {
        std::cout<<"\n Datos incorrectos no se encuentra el origen o destino.";
        return;
    }
    gDis.ori = orig;
    gDis.des = dest;
    gDis.pesoT = v;
    fp2.write(reinterpret_cast<char *> (&gDis), sizeof(gDis));
    struct calle *nna = new calle(v);
    nna->otraVia = Or->camino;
    nna->destino=De;
    Or->camino = nna;
}

void sacarCiudad()
{
    std::fstream temp("\GPS_P2\Ciudades.txt", std::ios::in | std::ios::out |std::ios::binary);
    temp.seekg(0);
    while(temp.eof() != true)
    {
        temp.read(reinterpret_cast<char *> (&gDis2), sizeof(gDis2));
        if(!temp.eof())
            llenarCiudad(gDis2.cod,gDis2.ori,gDis2.x,gDis2.y);
    }
    temp.close();
}

void sacarCaminos()
{
    std::fstream temp("\GPS_P2\Caminos.txt", std::ios::in | std::ios::out |std::ios::binary);
    temp.seekg(0);
    while(!temp.eof())
    {

        temp.read(reinterpret_cast<char *> (&gDis), sizeof(gDis));
        if(temp.eof() != true)
            costruirCamino(gDis.ori,gDis.ori,gDis.pesoT);
    }
    temp.close();
}

void datosPrueba()
{
    llenarCiudad("cr","CostaRica",590,550);
    llenarCiudad("ni","Nicaragua",575,525);
    llenarCiudad("pa","Panama",620,560);
    llenarCiudad("us","USA",430,317);
    llenarCiudad("me","Mexico",448,447);
    llenarCiudad("gu","Guatemala",530,502);
    llenarCiudad("sa","Salvador",545,522);
    llenarCiudad("cu","Cuba",609,452);
    llenarCiudad("ca","Canada",380,155);
    llenarCiudad("ar","Argentina",765,919);

    costruirCamino("CostaRica","Nicaragua",50);
    costruirCamino("CostaRica","Panama",80);
    costruirCamino("Nicaragua","CostaRica",50);
    costruirCamino("Nicaragua","Guatemala",100);
    costruirCamino("Nicaragua","Salvador",80);
    costruirCamino("Panama","CostaRica",80);
    costruirCamino("Panama","Argentina",150);
    costruirCamino("Salvador","Mexico",40);
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
}

bool existeRuta = false;
void hayRuta(std::string origen, std::string destino)
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
    struct calle *tempA = o->camino;
    while(tempA != NULL)
    {
        hayRuta(tempA->destino->nombre, destino);
        tempA = tempA->otraVia;
    }
}

//cuenta las rutas que existen del origen al destino.
int contRutas = 0 ;
void contarRutas (std::string origen, std::string destino)
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

std::vector<sf::Vector2f> CaminoCorto;
int peso,pesoTemp,pesoMe;
void dijkstra(std::string origen,std::string destino)
{
    struct ciudad *o = buscarCi(origen);

    if((o == NULL) or (o->visitado == true))
        return;

    if(origen == destino)
    {
        peso = pesoTemp;
        contRutas++;
        return;
    }
    o->visitado = true;
    CaminoCorto.push_back(o->cordenada);
    struct calle *tempA = o->camino;
    while(tempA != NULL)
    {
        pesoTemp +=tempA->distancia;
        contarRutas(tempA->destino->nombre, destino);
        tempA = tempA->otraVia;
    }
    if(pesoMe<peso)
    {
        pesoMe=peso;
    }
    o->visitado = false;
}

void validarGrafo()
{
    if(mundo==NULL)
    {
        sacarCaminos();
        sacarCiudad();
    }
}


// Graficos
int CB = 0;
void crearVentana()
{
    enum Direction {Down,Left,Right,Up};
    sf::Vector2i source(1,Down); //Direccion en la que ve la animacion.


    // Crear ventana
    sf::RenderWindow window;
    //sf::VideoMode DesktopMode = sf::VideoMode::getDesktopMode();
    //window.create(sf::VideoMode(1250,1284), "International Traveler");//,sf::Style::Close);
    window.create(sf::VideoMode(900,700), "International Traveler");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(10); // Controla los frames de la ventana (animacion)

    //Texturas y sus Sprites
    sf::Texture mapa;
    mapa.loadFromFile("data/mapa2.png");
    sf::Sprite background;
    background.setTexture(mapa);

    sf::Texture animacion; // Declaracion de variable Texture, la base.
    animacion.loadFromFile("data/animacion.png"); // Carga la imagen desde disco
    sf::Sprite playerImage; // Declaracion del Sprite
    playerImage.setTexture(animacion); // mete el Sprite dentro de la textura. Una textura puede tener varios sprites.

    sf::View view1(sf::FloatRect(-10, 0, 1250, 1300)); // Ajusta el ancho y alto de la imagen segun la ventana.
    window.setView(view1); // Asigna la vista con los ajustes a la ventana


    // Text para los codigos
    sf::String strCod = "Digite para ir:\n";
    strCod+="\n [ar]  Argentina";
    strCod+="\n [ca]  Canada";
    strCod+="\n [cr]  Costa Rica";
    strCod+="\n [cu]  Cuba";
    strCod+="\n [sa]  El Salvador";
    strCod+="\n [us]  Estados Unidos";
    strCod+="\n [gu]  Guatemala";
    strCod+="\n [me]  Mexico";
    strCod+="\n [ni]  Nicaragua";
    strCod+="\n [pa]  Panama";
    sf::Text txtPaises;
    sf::Font fuente;
    if (!fuente.loadFromFile("data/fuentes/OpenSans_Regular.ttf"))
    {
        std::cout<<"Error al cargar fuente de texto";
    }
    txtPaises.setString(strCod);
    txtPaises.setFont(fuente);
    txtPaises.setCharacterSize(20);
    txtPaises.setColor(sf::Color::Black);
    txtPaises.setPosition(sf::Vector2f(1010,50));

    // Entrada de datos.

    sf::Text txtin;
    sf::Font fuente2;
    if (!fuente2.loadFromFile("data/fuentes/OpenSans_Bold.ttf"))
    {
        std::cout<<"Error al cargar fuente de texto";
    }
    std::string input;
    sf::String mostrar;
    txtin.setFont(fuente2);
    txtin.setCharacterSize(20);
    txtin.setColor(sf::Color::Blue);
    txtin.setPosition(sf::Vector2f(1010,0));

    //Posicion actual.
    sf::Text txtActual;
    sf::String posActual= "Posicion: ";
    txtActual.setString(posActual);
    txtActual.setFont(fuente2);
    txtActual.setCharacterSize(18);
    txtActual.setColor(sf::Color::Green);
    txtActual.setPosition(sf::Vector2f(1010,750));

    // Texto con instrucciones.
    sf::Text txtInst;
    sf::String Ins= "Lista Instrucciones:\n[cod+espacio+Inst]\n";
    Ins+="\n [i]  Definir Inicio";
    Ins+="\n [r]  Buscar Ruta Corta";
    txtInst.setString(Ins);
    txtInst.setFont(fuente2);
    txtInst.setCharacterSize(20);
    txtInst.setColor(sf::Color::Magenta);
    txtInst.setPosition(sf::Vector2f(1010,800));

    // Bucle del juego

    while (window.isOpen())
    {
        window.draw(background); // Dibujamos el mapa en la ventana


        // Bucle de Eventos de ventana
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed: // Si se da click en boton cerrar.
                window.close();
                break;
            case sf::Event::KeyPressed:

                if(event.key.code==sf::Keyboard::Return)
                {

                    size_t tam = input.length()-1;

                    if(input.at(tam) == 'i') // Establecer posicion inicial
                    {
                        input.erase(tam-1); // -1 para borrar el espacio
                        struct ciudad*enc = buscarCodCi(input);
                        if(enc->cordenada!=sf::Vector2f(0,0) && enc!=NULL)
                        {
                            playerImage.setPosition(sf::Vector2f(enc->cordenada.x-8,enc->cordenada.y-8));
                            posActual= "Posicion: "+enc->nombre;
                            txtActual.setString(posActual);
                        }
                    }
                    else if(input.at(tam) == 'r') // Buscar Ruta Corta
                    {
                        // Ruta Corta
                    }
                }
                else if(event.key.code==sf::Keyboard::BackSpace)
                {
                    if(CB!=0)
                    {
                        input.erase(CB-1);
                        std::cout<<"\n"<<input;
                        mostrar = "Input: "+input;
                        txtin.setString(mostrar);
                    }
                }
                else if(event.key.code==sf::Keyboard::Up)
                {
                    source.y=Up; //Coloca animacion hacia arriba
                }

                else if(event.key.code==sf::Keyboard::Down)
                {
                    source.y=Down; //Coloca animacion hacia abajo
                }

                else if(event.key.code==sf::Keyboard::Right)
                {
                    source.y=Right; //Coloca animacion hacia la derecha
                }

                else if(event.key.code==sf::Keyboard::Left)
                {
                    source.y=Left; //Coloca animacion hacia la izquierda
                }

                break; // break del caso keyPressed
            case sf::Event::MouseButtonPressed:
                if(event.mouseButton.button==sf::Mouse::Left)
                {
                    std::cout<<"\nMouse: ("<<event.mouseButton.x<<" , "<<event.mouseButton.y<<")";
                }
                break; // break del caso MouseButtonPressed
            case sf::Event::TextEntered:
                if(event.text.unicode < 128)
                {
                    input += static_cast<char>(event.text.unicode);
                    size_t tam = input.length()-1;
                    CB=tam;
                    mostrar = "Input: "+input;
                    txtin.setString(mostrar);
                }
                break;
            }
        }

        //
        for (std::vector<Linea>::iterator it = Ruta.begin(); it != Ruta.end(); ++it)  // Dibuja las lineas
        {
            sf::RectangleShape rectCaja((*it).m_Size);
            rectCaja.setPosition((*it).m_Pos);
            rectCaja.setFillColor(sf::Color::Red);
            window.draw(rectCaja); //Dibujamos el elemento en el buffer
        }

        // Ciclo que pinta las banderas dentro del vector.
        for (std::vector<Bandera>::iterator it2 = Banderas.begin(); it2 != Banderas.end(); ++it2)  // Dibuja las banderas
        {
            //sf::RectangleShape recBandera((*it2).m_Size);
            //recBandera.setPosition((*it2).m_Pos);
            //recBandera.setFillColor(sf::Color::Green);
            //window.draw(recBandera); //Dibujamos el elemento en el buffer
            sf::Texture BAM;
            BAM.loadFromFile("data/sign2.png");
            sf::Sprite img;
            img.setTexture(BAM);
            img.setPosition((*it2).m_Pos);
            window.draw(img);
        }


        // Animacion.
        if(source.x * 32 >=animacion.getSize().x)
        {
            source.x=0;
        }
        else
        {
            source.x +=32;
        }
        sf::IntRect rect(source.x,source.y*32,32,32);// Coloca el segmento de la animacion en un rectangulo
        //para luego setearlo a textura mostrarlo como textura.
        playerImage.setTextureRect(sf::IntRect(source.x,source.y*32,32,32));
        window.draw(playerImage); //Dibuja en la ventana el Sprite playerImage
        window.draw(txtPaises); // Pintamos las intrucciones de paises

        if(CB==0)
        {
            mostrar = "Input: ";
            txtin.setString(mostrar);
        }
        window.draw(txtin);// Pintamos lo que digita el usuario.
        window.draw(txtInst);// Pintamos lo que digita el usuario.
        window.draw(txtActual); //Pintamos pais actual
        window.display(); // Mostrar el buffer en pantalla
        window.clear(sf::Color::White);

    }
}

int main()
{
    datosPrueba();
    dijkstra("CostaRica","Mexico");
    crearVentana();

    return 0;
}
