/**
 * 编译: gcc register_fundamental_1.c  `pkg-config --cflags --libs gobject-2.0 glib-2.0`
 */
#include <glib.h>
#include <glib-object.h>

int main(int argc, char *argv[]) {
    // g_type_init(); // 2.36后已经废弃
    
    GTypeInfo my_type_info = {
        0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, NULL
    };

    GTypeFundamentalInfo my_fundamental_info = {0};

    GType my_type_id = g_type_register_fundamental(
        g_type_fundamental_next(),
        "MyFirstFundamentalType",
        &my_type_info,
        &my_fundamental_info,
        0
    );

    g_print("%s\n", g_type_name(my_type_id));
    g_print("%s\n", g_type_name(G_TYPE_INT));
    g_print("%s\n", g_type_name(G_TYPE_FLOAT));
    g_print("%s\n", g_type_name(G_TYPE_OBJECT));

    g_print("Type id:%d\n", g_type_from_name("MyFirstFundamentalType"));
    g_print("Type name:%s\n", g_type_name(g_type_from_name("MyFirstFundamentalType")));

    g_print("Is abstract: %s\n",    G_TYPE_IS_ABSTRACT(my_type_id) ? "yes" : "no");
    g_print("Is derived: %s\n",     G_TYPE_IS_DERIVED(my_type_id) ? "yes" : "no");
    g_print("Is fundamental: %s\n", G_TYPE_IS_FUNDAMENTAL(my_type_id) ? "yes" : "no");
    g_print("Is value type: %s\n",  G_TYPE_IS_VALUE_TYPE(my_type_id) ? "yes" : "no");
    g_print("Has value table? %s\n", G_TYPE_HAS_VALUE_TABLE   (my_type_id)? "yes":"no");
    g_print("Is classed? %s\n",      G_TYPE_IS_CLASSED        (my_type_id)? "yes":"no");
    g_print("Is instantiable? %s\n", G_TYPE_IS_INSTANTIATABLE (my_type_id)? "yes":"no");
    g_print("Is derivable? %s\n",    G_TYPE_IS_DERIVABLE      (my_type_id)? "yes":"no");
    g_print("Is deep derivable? %s\n",G_TYPE_IS_DEEP_DERIVABLE(my_type_id)? "yes":"no");
    g_print("Is interface? %s\n",    G_TYPE_IS_INTERFACE      (my_type_id)? "yes":"no");
    return 0;
}
