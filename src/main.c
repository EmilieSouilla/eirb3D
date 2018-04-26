#include "window.h"
#include "events.h"
#include "vector.h"
#include "geometry.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main( int argc, char ** argv ) {

	const int width		= 1024;
	const int height	= 768;

	//ModelLoad("bin/data/head.obj");


	// Ouverture d'une nouvelle fenêtre
	window_t * mainwindow = WindowInit( width, height, 4 );

	int done = false;
	// Tant que l'utilisateur de ferme pas la fenêtre
	while ( !done ) {

		// Mise à jour et traitement des evênements de la fenêtre
		done = EventsUpdate( mainwindow );
		char* c = "bin/data/head.obj";
		ModelLoad(c);

		/*
		// Effacement de l'écran avec une couleur
		//WindowDrawClearColor( mainwindow, 64, 64, 64 );

		// Dessin d'un point blanc au milieu de le fenêtre
		//WindowDrawPoint( mainwindow, width / 2, height / 2, 255, 255, 255 );

		//Dessin d'une ligne
		//WindowDrawLine(mainwindow, 1000, 5, 200, 700 , 255, 255, 255);
*/
		int i, indA, indB, indC;
		float xA, xB, xC, yA, yB, yC;
		vector_t * f = ModelFaces();
		vector_t * vertice = ModelVertices();


		for(i = 0; i < 100 ; i++){

			face_t * face_de_ta_mere = (face_t *)VectorGetFromIdx(f, 2);

			indA = face_de_ta_mere->v[0];
			indB = face_de_ta_mere->v[1];
			indC = face_de_ta_mere->v[2];

			vec3f_t * A = (vec3f_t *)vertice->data[indA-1];
			vec3f_t * B = (vec3f_t *)vertice->data[indB-1];
			vec3f_t * C = (vec3f_t *)vertice->data[indC-1];

			xA = A->x;
			yA = A->y;
			xB = B->x;
			yB = B->y;
			xC = C->x;
			yC = C->y;
		}

/*
			printf("xA=%f, yA=%f \n",xA, yA);
			printf("xB=%f, yB=%f \n",xB, yB);
			printf("xC=%f, yC=%f \n",xC, yC);
		}
/*
			printf("indA=%d indB=%d indC=%d \n", indA, indB, indC);
*/

			WindowDrawLine(mainwindow, xA, yA, xB, yB, 255, 255, 255);
			WindowDrawLine(mainwindow, xB, yB, xC, yC, 255, 255, 255);
			WindowDrawLine(mainwindow, xC, yC, xA, yA, 255, 255, 255);


		printf("nique toi \n");
		// Mise à jour de la fenêtre
		WindowUpdate( mainwindow );

	}

	// Fermeture de la fenêtre
	WindowDestroy( mainwindow );

	return 1;
}
