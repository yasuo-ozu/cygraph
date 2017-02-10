#include "cg_common.h"

#ifdef cg_graphic
#undef cg_graphic
#endif

typedef struct {
	FILE *fp;


} cg_graphic;

cg_graphic *cg_graphic_init(FILE *fp, const int width, const int height) {
	cg_graphic *g = (cg_graphic *) calloc(1, sizeof(cg_graphic));
	g->fp = fp;
	fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\""
			"xmlns:xlink=\"http://www.w3.org/1999/xlink\""
			" width= \"%d\" height=\"%d\">\n", width, height);
	return g;
}

void cg_graphic_drawline(cg_graphic *g, const double x0, const double y0, const double x1, const double y1) {
	fprintf(g->fp, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"black\" stroke-width=\"1\"", x0, y0, x1, y1);
}

void cg_graphic_destroy(cg_graphic *g) {
	if (g->fp != stdout) fclose(g->fp);
	free(g);
}
