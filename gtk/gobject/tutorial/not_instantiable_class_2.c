#include <glib.h>
#include <glib-object.h>

typedef struct {
    GTypeClass something_as_boilerplate;
    int foo;
    char *bar;
    void (*baz)();
}myclass_t;

void baz_function() {
    g_print("All your type are belong to us!\n");
}

void my_class_init_func(myclass_t *klass, gpointer *data) {
    g_print("%s called!\n", __func__);
    klass->foo = 42;
    klass->bar = "Hello World!";
    klass->baz = baz_function;
}

int main(int argc, char *argv[]) {
    GTypeInfo my_type_info = {
        sizeof(myclass_t),  // guint16 class_size
        NULL,   // GBaseInitFunc base_init
        NULL,   // GBaseFinalizeFunc base_finalize

        /* classed types, instantiated types */
        (GClassInitFunc)my_class_init_func, // GClassInitFunc class_init
        NULL,   // GClassFinalizeFunc class_finalize
        NULL,   // gconstpointer class_data

        /* instantiated types */
        0,      // guint16 instance_size
        0,      // guint16 n_preallocs
        NULL,   // GInstanceInitFunc instance_init

        /* value handling */
        NULL,   // const GTypeValueTable *value_table
    };

    GTypeFundamentalInfo my_fundamental_info = {
        G_TYPE_FLAG_CLASSED
    };

    GType my_type_id = g_type_register_fundamental(
        g_type_fundamental_next(),
        "MyClassedFundamentalType",
        &my_type_info,
        &my_fundamental_info,
        0
    );

    g_print("%d\n", my_type_id);
    g_print("%s\n", g_type_name(my_type_id));

    myclass_t *klass = (myclass_t*)g_type_class_ref(my_type_id);
    g_print("foo:%d\n", klass->foo);
    g_print("bar:%s\n", klass->bar);
    klass->baz();

    g_type_class_unref(klass);

    return 0;
}

