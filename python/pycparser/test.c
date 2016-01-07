main ( int arc, char **argv ) {

    int i = 1, t = 2;

    if (i > 1) {
        if (2 == i) {
            printf("Yay!\n");
        }
    }

    for (i=0; i<2; i++) {
        for (;t<3;t++) {
            printf("Loop %d %d\n", i, t);
        }
    }

    // code
    return 0; // Indicates that everything vent well.     
}
