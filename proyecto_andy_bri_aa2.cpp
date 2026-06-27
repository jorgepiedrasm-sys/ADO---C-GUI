#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream> //creación del archivo

using namespace std;

int destinoSeleccionado = -1;
int seleccionado = -1; //para los asientos
int opcmenu=-1;
bool esRedondo = false;
//declarar el estado de los asientos de cada uno de los autobuses
bool bus_veracruz[20] = {false}, bus_cdmx[20] = {false}, bus_tlaxcala[20] = {false}, bus_oaxaca[20] = {false}, bus_jalisco[20] = {false};

void menu();



struct boleto
{
	char nombre[50], correo[50], telefono[12], destino[50], horario[20],tipo_viaje[20], asiento[4];
	float total;
	int destinoSeleccionado; //es int para distinguir el bus en cancelaciones, no va a en la impresion
    int posicionArray; //guardo la posicion del asiento en un array, no se muestra al usuario
	bool asistencia_medica;
	boleto *siguiente;    
};

boleto* cabeza = NULL; // puntero global 

/*
void agregarBoleto(boleto* nuevo_boleto) {
    if (cabeza == NULL) {
        cabeza = nuevo_boleto;  // Si la lista está vacía, el nuevo boleto es la cabeza
    } else {
        boleto* temp = cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;  // Ir al final de la lista
        }
        temp->siguiente = nuevo_boleto;  // Agregar el nuevo boleto al final
    }
}
*/

void agregarBoleto(boleto* nuevo_boleto) {
    boleto* aux1 = cabeza;  // Empezamos con la cabeza de la lista
    boleto* aux2 = NULL;

    // Si la lista está vacía o el nuevo boleto debe ir al principio
    if (cabeza == NULL || strcmp(nuevo_boleto->nombre, cabeza->nombre) < 0) {
        nuevo_boleto->siguiente = cabeza;  // El nuevo boleto será la cabeza
        cabeza = nuevo_boleto;  // Actualizamos la cabeza de la lista
    } else {
        // Recorremos la lista hasta encontrar la posición correcta para insertar el boleto
        while (aux1 != NULL && strcmp(aux1->nombre, nuevo_boleto->nombre) < 0) {
            aux2 = aux1;        // Guardamos el nodo anterior
            aux1 = aux1->siguiente;  // Avanzamos al siguiente nodo
        }

        // Insertamos el nuevo nodo entre aux2 y aux1
        aux2->siguiente = nuevo_boleto;
        nuevo_boleto->siguiente = aux1;
    }
}



//para escanear strings de cualquier tamaño y que me permita borrar
string capturarString(int x, int y) {
    string letra = "";
    char ch;

    while (true) {
        ch = getch();  // Captura un carácter sin mostrarlo en la consola

        if (ch == 13 && !letra.empty()) break;  // Código ASCII del Enter es 13, si ingreso Enter, termino de leer la cadena
        if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || 
            ch == '@' || ch == '.' || ch == '-' || ch == '_' || ch == ' ' || ch == '+' ) {  
            letra += ch; // Concatenar el carácter a la cadena

            // Borrar el área donde se muestra el texto para actualizarlo
            setfillstyle(SOLID_FILL, BLACK);
            bar(x, y, x + 300, y + 30);  // Borra el área del texto

            // Mostrar el texto actualizado en pantalla
            settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
            outtextxy(x, y, (char*)letra.c_str());
        } 
        else if (ch == 8 && !letra.empty()) {  // Si presionamos Backspace (código ASCII 8)
            letra.erase(letra.size() - 1);   // Eliminar el último carácter

            // Borrar el área donde se muestra el texto para actualizarlo
            setfillstyle(SOLID_FILL, BLACK);
            bar(x, y, x + 300, y + 30);  // Borra el área del texto

            // Mostrar el texto actualizado en pantalla
            settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
            outtextxy(x, y, (char*)letra.c_str());
        }
    }

    return letra;
}

//mostrar resumen del boleto comprado
void mostrarResumen(boleto* b) {
    cleardevice();  // Limpia la pantalla
	
	// se declaran las variables auxiliares para la conversión de la info que se guarda de nuevo_boleto
    char nombreTexto[100], destinoTexto[100], cantidadTexto[50], asistenciaTexto[50], totalTexto[50];
	
	// Dibujar un rectángulo alrededor del resumen
    rectangle(40, 40, 500, 250);
    
	setcolor(WHITE);
    settextstyle(EUROPEAN_FONT, HORIZ_DIR, 2);

    outtextxy(50, 50, "Resumen del Boleto");

    sprintf(nombreTexto, "Nombre: %s", b->nombre); //imprimir cadenas char, sin necesidad de colocar un for
    sprintf(destinoTexto, "Destino: %s", b->destino);
    //sprintf(cantidadTexto, "Cantidad de pasajeros: %d", b->cantidad_pasajeros);
    if (b->asistencia_medica) 
	{
    	sprintf(asistenciaTexto, "Asistencia medica: Si");
	} 
	else 
	{
    	sprintf(asistenciaTexto, "Asistencia medica: No");
	}
    sprintf(totalTexto, "Total a pagar: $%.2f", b->total);

    // se muestra la info del resumen con variables corregidas
    outtextxy(50, 100, nombreTexto);
    outtextxy(50, 130, destinoTexto);
    outtextxy(50, 160, asistenciaTexto);
    outtextxy(50, 190, totalTexto);

}

//Meter los datos del cliente
void registro_cliente(boleto* nuevo_boleto)
{
    cleardevice();  // Limpia la pantalla
    setcolor(WHITE);
    rectangle(40, 45, 600, 450);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);

    outtextxy(60, 60, "Favor de ingresar los siguientes datos:");

    // Pedir nombre
    outtextxy(60, 90, "Nombre:");
    strcpy(nuevo_boleto->nombre, capturarString(190, 90).c_str()); // <- Separa input del texto

    // Pedir correo
    outtextxy(60, 120, "Correo:");
    strcpy(nuevo_boleto->correo, capturarString(190, 120).c_str());

    // Pedir teléfono
    outtextxy(60, 150, "Telefono:");
    strcpy(nuevo_boleto->telefono, capturarString(190, 150).c_str());

	/*
    // Pedir cantidad de pasajeros
    bar(60, 180, 600, 200); // Limpia zona
	outtextxy(60, 180, "Cantidad de pasajeros:");
    nuevo_boleto->cantidad_pasajeros = stoi(capturarString(300, 180)); // -> Más a la derecha
    char cantidadPasajerosTexto[10];
    sprintf(cantidadPasajerosTexto, "%d", nuevo_boleto->cantidad_pasajeros);
	*/
	
    // Pedir asistencia médica
     bar(60, 220, 600, 240); // Limpia zona
	outtextxy(60, 220, "Requiere asistencia medica? (Si/No):");
    string respuesta = capturarString(300, 250);
    nuevo_boleto->asistencia_medica = (respuesta == "Si" || respuesta == "si");

    // Calcular total
    //nuevo_boleto->total = nuevo_boleto->precio * nuevo_boleto->cantidad_pasajeros;
    
    if (nuevo_boleto->asistencia_medica) {
        nuevo_boleto->total += 500;  // Asistencia médica adicional
    }

    // Guardar boleto y mostrar resumen
    agregarBoleto(nuevo_boleto);
    mostrarResumen(nuevo_boleto);
    delay(6000);
    menu();
}

//el encabezado del sistema
void sistema_abn()
{
	settextstyle(SCRIPT_FONT, HORIZ_DIR, 5);
    outtextxy(250, 30, "Sistema");

    settextstyle(GOTHIC_FONT, HORIZ_DIR, 4);
    outtextxy(270, 80, "ABN");
}

// TODOS LOS VOIDS PARA LA PARTE DE ASIENTOS

char obtenerLetraColumna(int col) {
    switch (col) {
        case 0: return 'A';
        case 1: return 'B';
        case 2: return 'C';
        case 3: return 'D';
        default: return '?'; // HUH?
    }
}

void dibujarAsientos() {
    int inicioX = 100;
    int inicioY = 150;
    int separacionX = 50; 
    int separacionY = 50;

    int num_asiento = 0;
    
    for (int fila = 0; fila < 5; ++fila) {
        // Lado izquierdo (A y B)
        for (int col = 0; col < 2; ++col) {
            if (num_asiento >= 20) break;

            int x = 100 + col * 50;
            int y = 150 + fila * 50;      
			
			//dependiendo de la seleccion, checa el estado de los autobuses
			//de veracruz
			if(destinoSeleccionado == 0)
		    {
		        if (num_asiento == seleccionado) 
				{
    				setfillstyle(SOLID_FILL, MAGENTA);
				}
				else if (bus_veracruz[num_asiento]) 
				{
    				setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else 
				{
    				setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    // de cdmx
		    else if (destinoSeleccionado == 1)
		    {
		        if (num_asiento == seleccionado) 
				{
    				setfillstyle(SOLID_FILL, MAGENTA);
				}
				else if (bus_cdmx[num_asiento]) 
				{
    				setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else 
				{
    				setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    //de tlaxcala
		    else if (destinoSeleccionado == 2)
		    {
		        if (num_asiento == seleccionado) 
				{
    				setfillstyle(SOLID_FILL, MAGENTA);
				}
				else if (bus_tlaxcala[num_asiento]) 
				{
    				setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else 
				{
    				setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    // de oaxaca
		    else if (destinoSeleccionado == 3)
		    {
		        if (num_asiento == seleccionado) 
				{
    				setfillstyle(SOLID_FILL, MAGENTA);
				}
				else if (bus_oaxaca[num_asiento]) 
				{
    				setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else 
				{
    				setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    //de jalisco
		    else if (destinoSeleccionado == 4)
		    {
		        if (num_asiento == seleccionado) 
				{
    				setfillstyle(SOLID_FILL, MAGENTA);
				}
				else if (bus_jalisco[num_asiento]) 
				{
    				setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else 
				{
    				setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }

            
			bar(x, y, x + 40, y + 40);

            // Borde blanco
            setcolor(WHITE);
            rectangle(x, y, x + 40, y + 40);

            // Texto del número (A1, B1, etc.)
            setcolor(WHITE);
            setbkcolor(BLACK);
            char etiqueta[4];
            settextstyle(EUROPEAN_FONT, HORIZ_DIR, 1);
            sprintf(etiqueta, "%c%d", obtenerLetraColumna(num_asiento % 4), num_asiento / 4 + 1);
            outtextxy(x + 5, y + 12, etiqueta);

            num_asiento++;
        }

        // Lado derecho (C y D)
        for (int col = 2; col < 4; ++col) {
            if (num_asiento >= 20) 
			{
				break;
			}

            int x = 100 + (col + 1) * 50; // +1 para pasillo
            int y = 150 + fila * 50;

            //dependiendo de la seleccion, checa el estado de los autobuses
			//de veracruz
			if(destinoSeleccionado == 0)
		    {
		        if (bus_veracruz[num_asiento]) 
				{
					setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else if (num_asiento == seleccionado) 
				{
					setfillstyle(SOLID_FILL, MAGENTA);
				}
            	else 
				{
					setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    // de cdmx
		    else if (destinoSeleccionado == 1)
		    {
		        if (bus_cdmx[num_asiento]) 
				{
					setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else if (num_asiento == seleccionado) 
				{
					setfillstyle(SOLID_FILL, MAGENTA);
				}
            	else 
				{
					setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    //de tlaxcala
		    else if (destinoSeleccionado == 2)
		    {
		        if (bus_tlaxcala[num_asiento]) 
				{
					setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else if (num_asiento == seleccionado) 
				{
					setfillstyle(SOLID_FILL, MAGENTA);
				}
            	else 
				{
					setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    // de oaxaca
		    else if (destinoSeleccionado == 3)
		    {
		        if (bus_oaxaca[num_asiento]) 
				{
					setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else if (num_asiento == seleccionado) 
				{
					setfillstyle(SOLID_FILL, MAGENTA);
				}
            	else 
				{
					setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }
		    //de jalisco
		    else if (destinoSeleccionado == 4)
		    {
		        if (bus_jalisco[num_asiento]) 
				{
					setfillstyle(SOLID_FILL, DARKGRAY);
				}
				else if (num_asiento == seleccionado) 
				{
					setfillstyle(SOLID_FILL, MAGENTA);
				}
            	else 
				{
					setfillstyle(SOLID_FILL, LIGHTGRAY);
				}
		    }

            bar(x, y, x + 40, y + 40);

            // Borde blanco
            setcolor(WHITE);
            rectangle(x, y, x + 40, y + 40);

            // Texto del número (C1, D1, etc.)
            setcolor(WHITE);
            setbkcolor(BLACK);
            char etiqueta[4];
            sprintf(etiqueta, "%c%d", obtenerLetraColumna(num_asiento % 4), num_asiento / 4 + 1);
            outtextxy(x + 5, y + 12, etiqueta);

            num_asiento++;
        }
    }
}

int obtenerAsientoPorCoordenada(int mx, int my) {
    int num_asiento = 0;

    for (int fila = 0; fila < 5; ++fila) {
        for (int col = 0; col < 2; ++col) {
            if (num_asiento >= 20) break;

            int x = 100 + col * 50;
            int y = 150 + fila * 50;

            if (mx >= x && mx <= x + 40 && my >= y && my <= y + 40)
                return num_asiento;

            num_asiento++;
        }

        for (int col = 2; col < 4; ++col) {
            if (num_asiento >= 20) break;

            int x = 100 + (col + 1) * 50;
            int y = 150 + fila * 50;

            if (mx >= x && mx <= x + 40 && my >= y && my <= y + 40)
                return num_asiento;

            num_asiento++;
        }
    }

    return -1;
}

void asientos(boleto* nuevo_boleto)
{
	seleccionado = -1; //reiniciar variable de los asientos
	setbkcolor(BLACK); // fondo negro
    cleardevice();
    dibujarAsientos();

    setcolor(WHITE);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(164, 70, "Seleccione el asiento:");

    int mx, my;
    while (!kbhit()) 
	{
    	
        if (ismouseclick(WM_LBUTTONDOWN)) 
		{
            getmouseclick(WM_LBUTTONDOWN, mx, my);
            int i = obtenerAsientoPorCoordenada(mx, my);
            
            //veracruz
            if(destinoSeleccionado == 0)
		    {
		        if (i != -1 && !bus_veracruz[i]) 
				{
	                seleccionado = i;
	                //guardar en struct el asiento
	                sprintf(nuevo_boleto->asiento, "%c%d", obtenerLetraColumna(i % 4), i / 4 + 1);
					nuevo_boleto->posicionArray = seleccionado;
	                
	                //cambiar el estado de la matriz
	                bus_veracruz[seleccionado] = true;
	               	
	                cleardevice();
	                dibujarAsientos();
	
	                setcolor(WHITE);
				    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
				    outtextxy(164, 70, "Seleccione el asiento:");
				    
				    delay(1500);
				    break;
            	}
		    }
		    
		    //cdmx
		    else if (destinoSeleccionado == 1)
		    {
		        if (i != -1 && !bus_cdmx[i]) 
				{
	                seleccionado = i;
	                //guardar en struct el asiento
	                sprintf(nuevo_boleto->asiento, "%c%d", obtenerLetraColumna(i % 4), i / 4 + 1);
	                nuevo_boleto->posicionArray = seleccionado;
	                
	                //cambiar el estado de la matriz
	                bus_cdmx[seleccionado] = true;
	
	                cleardevice();
	                dibujarAsientos();
	
	                setcolor(WHITE);
				    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
				    outtextxy(164, 70, "Seleccione el asiento:");
				    
				    delay(1500);
				    break;
            	}
		    }
		    
		    //tlaxcala
		    else if (destinoSeleccionado == 2)
		    {
		        if (i != -1 && !bus_tlaxcala[i]) 
				{
	                seleccionado = i;
	                //guardar en struct el asiento
	                sprintf(nuevo_boleto->asiento, "%c%d", obtenerLetraColumna(i % 4), i / 4 + 1);
	                nuevo_boleto->posicionArray = seleccionado;
	                
	                //cambiar el estado de la matriz
	                bus_tlaxcala[seleccionado] = true;
	
	                cleardevice();
	                dibujarAsientos();
	
	                setcolor(WHITE);
				    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
				    outtextxy(164, 70, "Seleccione el asiento:");
				    
				    delay(1500);
				    break;
            	}
		    }
		    
		    //oaxaca
		    else if (destinoSeleccionado == 3)
		    {
		        if (i != -1 && !bus_oaxaca[i]) 
				{
	                seleccionado = i;
	                //guardar en struct el asiento
	                sprintf(nuevo_boleto->asiento, "%c%d", obtenerLetraColumna(i % 4), i / 4 + 1);
	                nuevo_boleto->posicionArray = seleccionado;
	                
	                //cambiar el estado de la matriz
	                bus_oaxaca[seleccionado] = true;
	
	                cleardevice();
	                dibujarAsientos();
	
	                setcolor(WHITE);
				    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
				    outtextxy(164, 70, "Seleccione el asiento:");
				    
				    delay(1500);
				    break;
            	}
		    }
		    
		    //jalisco
		    else if (destinoSeleccionado == 4)
		    {
		        if (i != -1 && !bus_jalisco[i]) 
				{
	                seleccionado = i;
	                //guardar en struct el asiento
	                sprintf(nuevo_boleto->asiento, "%c%d", obtenerLetraColumna(i % 4), i / 4 + 1);
	                nuevo_boleto->posicionArray = seleccionado;
	                
	                //cambiar el estado de la matriz
	                bus_jalisco[seleccionado] = true;
	
	                cleardevice();
	                dibujarAsientos();
	
	                setcolor(WHITE);
				    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
				    outtextxy(164, 70, "Seleccione el asiento:");
				    
				    delay(1500);
				    break;
            	}
		    }
            
        }
    }
    
    registro_cliente(nuevo_boleto);
	
}

//display escoger el tipo de viaje
void display_tipo_viajes()
{
    int viaje_seleccionado = 0;
    int gd = DETECT, gm, x, y;
    
    cleardevice();
    boleto *nuevo_boleto = new boleto();
    nuevo_boleto->siguiente = NULL;
    
    if(destinoSeleccionado == 0)
    {
        strcpy(nuevo_boleto->destino, "Veracruz");
        nuevo_boleto -> destinoSeleccionado = 0;
    }
    else if (destinoSeleccionado == 1)
    {
        strcpy(nuevo_boleto->destino, "CDMX");
        nuevo_boleto -> destinoSeleccionado = 1;
    }
    else if (destinoSeleccionado == 2)
    {
        strcpy(nuevo_boleto->destino, "Tlaxcala");
        nuevo_boleto -> destinoSeleccionado = 2;
    }
    else if (destinoSeleccionado == 3)
    {
        strcpy(nuevo_boleto->destino, "Oaxaca");
        nuevo_boleto -> destinoSeleccionado = 3;
    }
    else if (destinoSeleccionado == 4)
    {
        strcpy(nuevo_boleto->destino, "Jalisco");
        nuevo_boleto -> destinoSeleccionado = 4;
    }
    
    while(true)
    {
    	 cleardevice();
    	//frase de seleccionar tipo de viaje
    	setcolor(WHITE); 
    	settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
        outtextxy(145, 40, "Seleccione el tipo de viaje:");
        
        rectangle(35, 100, 300, 300);
        settextstyle(SCRIPT_FONT, HORIZ_DIR, 4);
        outtextxy(60, 120, "Viaje Redondo");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(90, 160, "Precio: $500");
        outtextxy(57, 180, "Salida: 08:00 AM");
        outtextxy(47, 200, "Llegada: 12:00 PM");
        outtextxy(75, 225, "Origen: Puebla");
        outtextxy(75, 245, "Destino: ");
        outtextxy(130, 265, nuevo_boleto->destino);
        
        // Dibujar el segundo rectángulo (Viaje Normal)
        if (viaje_seleccionado == 2) 
        {
            setcolor(LIGHTBLUE);
        } 
        else 
        {
            setcolor(WHITE);
        }
        rectangle(350, 100, 615, 300);
        settextstyle(SCRIPT_FONT, HORIZ_DIR, 4);
        outtextxy(385, 120, "Viaje Normal");
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(405, 160, "Precio: $300");
        outtextxy(372, 180, "Salida: 10:00 AM");
        outtextxy(362, 200, "Llegada: 02:00 PM");
        outtextxy(385, 225, "Origen: Puebla");
        outtextxy(385, 245, "Destino: ");
        outtextxy(445, 265, nuevo_boleto->destino);
        
        // Mouse click
        if (ismouseclick(WM_LBUTTONDOWN)) 
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            
            
            // Verificar si se ha hecho clic en Viaje Redondo
            if (x > 35 && x < 300 && y > 100 && y < 300) 
            {
                viaje_seleccionado = 1;
                strcpy(nuevo_boleto->tipo_viaje, "Redondo");
	        	nuevo_boleto -> total = 500;
	        	
				//Mostrar el recuadro seleccionado (cambiar el color)
				cleardevice();
		        setcolor(LIGHTBLUE);
		        rectangle(35, 100, 300, 300);
		        settextstyle(SCRIPT_FONT, HORIZ_DIR, 4);
		        outtextxy(60, 120, "Viaje Redondo");
		        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
		        outtextxy(90, 160, "Precio: $500");
		        outtextxy(57, 180, "Salida: 08:00 AM");
		        outtextxy(47, 200, "Llegada: 12:00 PM");
		        outtextxy(75, 225, "Origen: Puebla");
		        outtextxy(75, 245, "Destino: ");
		        outtextxy(130, 265, nuevo_boleto->destino);
		        delay(1500);

        		break;
	        	
            }
            // Verificar si se ha hecho clic en Viaje Normal
            else if (x > 350 && x < 615 && y > 100 && y < 300) 
            {
                viaje_seleccionado= 2;
                strcpy(nuevo_boleto->tipo_viaje, "Normal");
				nuevo_boleto -> total = 300;
				 
				//Mostrar el recuadro seleccionado
				cleardevice();
		        setcolor(LIGHTBLUE);
		        rectangle(350, 100, 615, 300);
		        settextstyle(SCRIPT_FONT, HORIZ_DIR, 4);
		        outtextxy(385, 120, "Viaje Normal");
		        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
		        outtextxy(405, 160, "Precio: $300");
		        outtextxy(372, 180, "Salida: 10:00 AM");
		        outtextxy(362, 200, "Llegada: 02:00 PM");
		        outtextxy(385, 225, "Origen: Puebla");
		        outtextxy(385, 245, "Destino: ");
		        outtextxy(445, 265, nuevo_boleto->destino);
		        delay(1500);

        		break;
				
            }
            
        }
        
		
        delay(100);
    }
    asientos(nuevo_boleto);
    //se ponen automaticamente los precios según escojas el tipo de viaje
	//registro_cliente(nuevo_boleto);
}

//display para escoger el destino 
void escoger_destino()
{
	int mx, my;
    
    while (true) {
    	destinoSeleccionado = -1; //para que se borre el seleccionado
        cleardevice();
        
        sistema_abn();
        
        setcolor(WHITE);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
        outtextxy(164, 150, "Seleccione el destino:");
        
        if (destinoSeleccionado == 0) setfillstyle(SOLID_FILL, RED);
        else setfillstyle(SOLID_FILL, GREEN);
        bar(100, 200, 230, 240);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(105, 210, "Veracruz");
        
        if (destinoSeleccionado == 1) setfillstyle(SOLID_FILL, RED);
        else setfillstyle(SOLID_FILL, GREEN);
        bar(250, 200, 380, 240);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(273, 210, "CDMX");
        
        if (destinoSeleccionado == 2) setfillstyle(SOLID_FILL, RED);
        else setfillstyle(SOLID_FILL, GREEN);
        bar(400, 200, 530, 240);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(410, 210, "Tlaxcala");
        
        if (destinoSeleccionado == 3) setfillstyle(SOLID_FILL, RED);
        else setfillstyle(SOLID_FILL, GREEN);
        bar(175, 260, 305, 300);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(190, 271 , "Oaxaca");
        
        if (destinoSeleccionado == 4) setfillstyle(SOLID_FILL, RED);
        else setfillstyle(SOLID_FILL, GREEN);
        bar(325, 260, 455, 300);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(345, 271, "Jalisco");
        
        if (ismouseclick(WM_LBUTTONDOWN)) 
		{
            getmouseclick(WM_LBUTTONDOWN, mx, my);
            if (mx >= 100 && mx <= 230 && my >= 200 && my <= 240) 
			{
				destinoSeleccionado = 0;
			}
            else if (mx >= 250 && mx <= 380 && my >= 200 && my <= 240) 
			{
				destinoSeleccionado = 1;
			}
            else if (mx >= 400 && mx <= 530 && my >= 200 && my <= 240) 
			{
				destinoSeleccionado = 2;
			}
            else if (mx >= 175 && mx <= 305 && my >= 260 && my <= 300)
			{
				 destinoSeleccionado = 3;
			}
            else if (mx >= 325 && mx <= 455 && my >= 260 && my <= 300) 
            {
            	destinoSeleccionado = 4;
			}
			
			// **Actualizar pantalla con el botón rojo**
		    cleardevice();
		    sistema_abn();
		    
		    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
		    outtextxy(164, 150, "Seleccione el destino:");
		
			//recorre los posibles estados de los botonos (si han sido seleccionados) lo hace uno por uno
		    for (int i = 0; i < 5; i++) 
		    {
		        if (destinoSeleccionado == i) setfillstyle(SOLID_FILL, RED);
		        else setfillstyle(SOLID_FILL, GREEN);
		
		        switch(i)
		        {
		            case 0: bar(100, 200, 230, 240); outtextxy(105, 210, "Veracruz"); break;
		            case 1: bar(250, 200, 380, 240); outtextxy(273, 210, "CDMX"); break;
		            case 2: bar(400, 200, 530, 240); outtextxy(410, 210, "Tlaxcala"); break;
		            case 3: bar(175, 260, 305, 300); outtextxy(190, 271, "Oaxaca"); break;
		            case 4: bar(325, 260, 455, 300); outtextxy(345, 271, "Jalisco"); break;
		        }
		    }
		    
		    // esperar 1.5 segundos para ver display_viaje_pantalla
			delay(1500);
		    display_tipo_viajes();
        }
        
        delay(100);
    }
}

//display para cancelaciones
void cancelaciones() 
{
    cleardevice();
    char nombre_buscado[50];
    
    setcolor(WHITE); 
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(100, 40, "Ingresa el nombre del pasajero:");
    strcpy(nombre_buscado, capturarString(120, 70).c_str()); 

    boleto *actual = cabeza; //actual es el nodo que nos ayuda a buscar 
    boleto *anterior = NULL;
    
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre_buscado) == 0) 
		{
            // Si se encuentra el nodo:
            // Si el nodo a borrar es la cabeza
            if (anterior == NULL) 
			{
                cabeza = actual->siguiente;
            } 
            // nodo enmedio de la lista 
			else 
			{
                anterior->siguiente = actual->siguiente;
            }
            
            if(actual->destinoSeleccionado == 0)
            {
            	bus_veracruz[actual->posicionArray] = false;
			}
			else if(actual->destinoSeleccionado==1)
			{
				bus_cdmx[actual->posicionArray] = false;
			}
			else if(actual->destinoSeleccionado==2)
			{
				bus_tlaxcala[actual->posicionArray] = false;
			}
			else if(actual->destinoSeleccionado==3)
			{
				bus_oaxaca[actual->posicionArray] = false;
			}
			else if(actual->destinoSeleccionado==4)
			{
				bus_jalisco[actual->posicionArray] = false;
			}
			
            
            delete actual;// Borrar el nodo
            
            cleardevice();
            outtextxy(60, 220, "Se ha eliminado el boleto con el nombre ");
            outtextxy(60, 240, nombre_buscado);  // Mostrar el nombre eliminado
            delay(3000);
            return;
        }
        
        // Avanzar al siguiente nodo
        anterior = actual;
        actual = actual->siguiente;
    }

    // Si no se encuentra el nombre
    outtextxy(60, 220, "No se ha encontrado el boleto con ese nombre.");
    delay(3000);
}


//display para buscar pasajeros
void buscar_pasajeros()
{
	cleardevice();
    char nombre_buscado[50];
    
    setcolor(WHITE); 
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(100, 40, "Ingresa el nombre del pasajero:");
    strcpy(nombre_buscado, capturarString(120, 70).c_str()); 

    boleto *actual = cabeza; //actual es el nodo que nos ayuda a buscar 
    
    while (actual != NULL) 
	{	// Si se encuentra el nodo:
        if (strcmp(actual->nombre, nombre_buscado) == 0) 
		{
            mostrarResumen(actual);
           	delay(6000);
            return;
        }
        
        // Avanzar al siguiente nodo
        actual = actual->siguiente;
    }

    // Si no se encuentra el nombre
    outtextxy(60, 220, "No se ha encontrado el boleto con ese nombre.");
    delay(3000);
	
}

//display para imprimir pasajeros

void imprimir() {
    cleardevice();  // Limpiar la pantalla

    boleto* temp = cabeza;  // Iniciamos con la cabeza de la lista
    int yPos = 50;  // Posición vertical donde comenzaremos a imprimir

    setcolor(WHITE);
    settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
    outtextxy(100, 30, "Lista de Pasajeros:");

    // Verificar si la lista está vacía
    if (temp == NULL) {
        outtextxy(100, 60, "No hay pasajeros registrados.");
        delay(3000);  // Esperar 3 segundos para que el usuario vea el mensaje
        return;
    }

    // Recorremos la lista y mostramos la información de cada boleto
    while (temp != NULL) {
        // Imprimir cada atributo por separado con salto de línea y buena alineación
        char nombre[100], destino[100], tipo_viaje[100], total[100], asistencia_medica[50];

        sprintf(nombre, "Nombre: %s", temp->nombre);
        sprintf(destino, "Destino: %s", temp->destino);
        sprintf(tipo_viaje, "Tipo de Viaje: %s", temp->tipo_viaje);
        sprintf(total, "Total a pagar: $%.2f", temp->total);
        sprintf(asistencia_medica, "Asistencia Medica: %s", temp->asistencia_medica ? "Si" : "No");

        // Imprimir cada línea por separado para una mejor visualización
        outtextxy(50, yPos, nombre);
        yPos += 30;
        outtextxy(50, yPos, destino);
        yPos += 30;
        outtextxy(50, yPos, tipo_viaje);
        yPos += 30;
        outtextxy(50, yPos, total);
        yPos += 30;
        outtextxy(50, yPos, asistencia_medica);
        yPos += 30;
        outtextxy(50, yPos, "--------------------------------------------");
        yPos += 40;  // Espacio adicional entre registros

        // Avanzar al siguiente boleto en la lista
        temp = temp->siguiente;

        // Si la lista es muy grande, pausar la impresión para no sobrecargar la pantalla
        if (yPos > getmaxy() - 50) {
            delay(3000);  // Esperar 3 segundos
            cleardevice();  // Limpiar pantalla para seguir imprimiendo
            yPos = 50;  // Reiniciar la posición vertical
        }
    }

    delay(3000);  // Esperar 3 segundos para que el usuario vea la lista completa
    menu();  // Volver al menú principal
}


// para imprimir los pasajeros en el archivo txt
void imprimir_archivo(boleto* cabeza) {
    FILE *fd;
    fd = fopen("boletos_registrados.txt", "a"); // Abrir archivo en modo añadir

    if (fd == NULL) {
        printf("No se puede crear el archivo \n");
        return;
    }

    fprintf(fd, "==== Lista de boletos registrados ====\n");

    boleto* actual = cabeza;
    while (actual != NULL) 
	{
        
		fprintf(fd, "Nombre: %s\n", actual->nombre);
        fprintf(fd, "Correo: %s\n", actual->correo);
        fprintf(fd, "Teléfono: %s\n", actual->telefono);
        fprintf(fd, "Destino: %s\n", actual->destino);
        fprintf(fd, "Horario: %s\n", actual->horario);
        fprintf(fd, "Tipo de Viaje: %s\n", actual->tipo_viaje);
        fprintf(fd, "Asiento: %s\n", actual->asiento);
        fprintf(fd, "Total a pagar: $%.2f\n", actual->total);
        fprintf(fd, "Asistencia Médica: %s\n", actual->asistencia_medica ? "Sí" : "No");
        fprintf(fd, "--------------------------------------------\n");

        actual = actual->siguiente;
    }

    fclose(fd); // Cerrar el archivo
}


//mensaje de acrhivo imprimir

void mensaje_archivo()
{
	// Limpiar pantalla y se muestra un mensaje de confirmación
    cleardevice();
	settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
    outtextxy(75, 200, "El archivo ha sido generado con exito");
    
    delay(3000); // Esperar 3 segundos para que el usuario vea el mensaje antes de volver al menú
}

//menu principal 
void menu()
{
	int mx, my;
    
    opcmenu=-1;
    
	while(true)
	{
		cleardevice();
		sistema_abn();
		settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
		outtextxy(174, 150, "Seleccione la opcion:");
		
		if (opcmenu==1) setfillstyle(SOLID_FILL, MAGENTA);
        else setfillstyle(SOLID_FILL, CYAN);
        bar(100, 200, 550, 240);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(150, 210, "1. Registrar Boleto");
       
        if (opcmenu==2) setfillstyle(SOLID_FILL, MAGENTA);
        else setfillstyle(SOLID_FILL, CYAN);
        bar(100, 250, 550, 290);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(150, 260, "2. Cancelaciones");
        
        if (opcmenu==3) setfillstyle(SOLID_FILL, MAGENTA);
        else setfillstyle(SOLID_FILL, CYAN);
        bar(100, 300, 550, 340);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(150, 310, "3. Buscar pasajeros");
         
        if (opcmenu==4) setfillstyle(SOLID_FILL, MAGENTA);
        else setfillstyle(SOLID_FILL, CYAN);
        bar(100, 350, 550, 390);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(150, 360, "4. Imprimir pasajeros");
        
        if(opcmenu==5) setfillstyle(SOLID_FILL, MAGENTA);
        else setfillstyle(SOLID_FILL, CYAN);
        bar(100,400,550,440);
        settextstyle(GOTHIC_FONT, HORIZ_DIR, 1);
        outtextxy(150, 410, "5. Imprimir archivo");
        
        if (ismouseclick(WM_LBUTTONDOWN)) 
		{
            getmouseclick(WM_LBUTTONDOWN, mx, my);
            if (mx >= 100 && mx <= 550 && my >= 200 && my <= 240) 
			{
			    opcmenu = 1;
			}
			else if (mx >= 100 && mx <= 550 && my >= 250 && my <= 290) 
			{
			    opcmenu = 2;
			}
			else if (mx >= 100 && mx <= 550 && my >= 300 && my <= 340) 
			{
			    opcmenu = 3;
			}
			else if (mx >= 100 && mx <= 550 && my >= 350 && my <= 390)
			{
			    opcmenu = 4;
			}
			
			else if (mx >= 100 && mx <= 550 && my >= 400 && my <= 440)
			{
			    opcmenu = 5;
			}

			// **Actualizar pantalla con el botón magenta**
		    cleardevice();
		    sistema_abn();
		    
		    settextstyle(GOTHIC_FONT, HORIZ_DIR, 2);
		    outtextxy(174, 150, "Seleccione el destino:");
		
			//recorre los posibles estados de los botonos (si han sido seleccionados) lo hace uno por uno
		    for (int i = 1; i <= 5; i++) 
		    {
		        if (opcmenu == i) setfillstyle(SOLID_FILL, MAGENTA);
		        else setfillstyle(SOLID_FILL, CYAN);
		
		        switch(i)
		        {
		            case 1: bar(100, 200, 550, 240); outtextxy(150, 210, "1. Registrar boleto"); break;
		            case 2: bar(100, 250, 550, 290); outtextxy(150, 260, "2. Cancelaciones"); break;
		            case 3: bar(100, 300, 550, 340); outtextxy(150, 310, "3. Buscar pasajeros"); break;
		            case 4: bar(100, 350, 550, 390); outtextxy(150, 360, "4. Imprimir pasajeros"); break;
		            case 5: bar(100, 400, 550, 440); outtextxy(150, 410, "5. Imprimir en archivo"); break;
		        }
		    }
		    
		    // esperar 1.5 segundos para ver display_viaje_pantalla
			delay(1500);
			
			switch(opcmenu)
			{
				case 1:
					escoger_destino();
				break;
				
				case 2:
					cancelaciones();
				break;
					
				case 3:
					buscar_pasajeros();
				break;
				
				case 4:
					imprimir();
				break;	
					
				case 5:
					imprimir_archivo(cabeza);
					mensaje_archivo();	
				break;
				
				default:
				break;
			}
        }
        	
	delay(100);
	}
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");
   
   	menu();
    
    getch();
    closegraph();
    return 0;
}