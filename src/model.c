#include "model.h"

vector_t * g_vertex;
vector_t * g_norm;
vector_t * g_texcoord;
vector_t * g_face;

vector_t * ModelVertices() {
	return g_vertex;
}

vector_t * ModelNormals() {
	return g_norm;
}

vector_t * ModelTexcoords() {
	return g_texcoord;
}

vector_t * ModelFaces() {
	return g_face;
}

vec3f_t ModelGetVertex( int index ) {
	vec3f_t v = *(vec3f_t*)VectorGetFromIdx( ModelVertices(), index );
	return v;
}

vec3f_t ModelGetNormal( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelNormals(), index );
	return t;
}

vec3f_t ModelGetTexcoord( int index ) {
	vec3f_t t = *(vec3f_t*)VectorGetFromIdx( ModelTexcoords(), index );
	return t;
}

face_t ModelGetFace( int index ) {
	face_t f = *(face_t*)VectorGetFromIdx( ModelFaces(), index );
	return f;
}

bool ModelLoad( char * objfilename ) {

	FILE * object = fopen(objfilename, "r");
	g_vertex   = Vector();
	g_norm     = Vector();
	g_texcoord = Vector();
	g_face     = Vector();

		if(object == NULL){
			printf("Erreur d'ouverture du fichier .obj \n");
		}
    else {

				char line[256];
				float x, y, z;
				int v0, v1, v2, vt0, vt1, vt2, vn0, vn1, vn2;
				char a[3];
				vec3f_t * vertex = (vec3f_t *)malloc (sizeof(vec3f_t));
				vertex->x = 0.0; vertex->y = 0.0; vertex->z = 0.0;
			  vec2f_t  * texture    = (vec2f_t  *)malloc(sizeof(vec2f_t));
				texture->x = 0; texture->y = 0;
				vec3f_t  * normale = (vec3f_t  *)malloc(sizeof(vec3f_t));
				normale->x = 0; normale->y = 0; normale->z = 0;
				face_t * f = (face_t *)malloc(sizeof(face_t));
				/*f->v[0]=0; f->v[1]=0; f->v[2]=0;
				f->vn[0]=0; f->vn[1]=0; f->vn[2]=0;
				f->vt[0]=0; f->vt[1]=0; f->vt[2]=0;
*/
int i=0;
				while(fgets(line, 255, object) != NULL){

					if(line[0] == 'v'){
						if(line[1] == ' '){

							sscanf(line,"v %f %f %f", &x, &y, &z);
							vertex->x = x;
							vertex->y = y;
							vertex->z = z;
							//printf("remplissage x=%f y=%f z=%f \n", x, y, z);
							VectorAdd(g_vertex, vertex);
						}

						else if(line[1] == 't'){

							sscanf(line, "vt %f %f", &x, &y);
							texture->x = x;
							texture->y = y;
							VectorAdd(g_texcoord, texture);
					  }

						else if(line[1] == 'n'){

							sscanf(line, "vn %f %f %f", &x, &y, &z);
							normale->x = x;
							normale->y = y;
							normale->z = z;
							VectorAdd(g_norm, normale);
						}
					}
					else if (line[0] == 'f'){

						sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v0, &vt0, &vn0, &v1, &vt1, &vn1, &v2, &vt2, &vn2);
						f->v[0] = v0; f->v[1] = v1; f->v[2]=v2;
						f->vn[0]=vn0; f->vn[1]=vn1; f->vn[2]=vn2;
						f->vt[0]=vt0; f->vt[1]=vt1; f->vt[2]=vt2;
						VectorAdd(g_face, f);
						printf("idx=%d, v1=%d v2=%d v3=%d \n", i, v0, v1, v2);
i++;
					}
				}
		fclose(object);

		}
/*
	int i;
	for(i=0; i<g_vertex->count ; i++){
		vec3f_t * vector_de_ta_mere = (vec3f_t *)VectorGetFromIdx( g_vertex, i );
		printf("idx=%d, x=%f y=%f z=%f \n", i, vector_de_ta_mere->x, vector_de_ta_mere->y, vector_de_ta_mere->z);
	}
	printf("nique ta mère \n");

*/
/*
int i;
for(i=0; i<g_face->count ; i++){
	face_t * face_de_ta_mere = (face_t *)VectorGetFromIdx( g_face, i );
	printf("idx=%d, v1=%d v2=%d v3=%d \n", i, face_de_ta_mere->v[0], face_de_ta_mere->v[1], face_de_ta_mere->v[2]);
	printf("idx=%d, vt1=%d vt2=%d vt3=%d \n", i, face_de_ta_mere->vt[0], face_de_ta_mere->vt[1], face_de_ta_mere->vt[2]);
	printf("idx=%d, vn1=%d vn2=%d vn3=%d \n", i, face_de_ta_mere->vn[0], face_de_ta_mere->vn[1], face_de_ta_mere->vn[2]);

}
*/
//printf("nique ta mère \n");



	return true;

}
