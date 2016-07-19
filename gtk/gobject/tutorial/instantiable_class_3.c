#include <glib.h>
#include <glib-object.h>

typedef struct {
    GTypeClass something_as_boilerplate;
} myclass_t;

int a_class_member_of_myclass;

typedef struct {
    GTypeInstance something_as_boilerplate;
    int an_instance_member;
} myinstance_t;

void my_class_method() {
    g_print("The class member is %d\n", a_class_member_of_myclass);
}

void my_instance_method(myinstance_t *instance, int a_parameter) {
    g_print("The member is %d\n", instance->an_instance_member);
    g_print("The parameter is %d\n", a_parameter);
}

void my_class_init_func(myclass_t *klass, gpointer *data) {
    g_print("%s called!\n", __func__);
    a_class_member_of_myclass = 42;
}

void my_instance_init_func(myinstance_t *instance, gpointer *data) {
    g_print("%s called!\n", __func__);
    instance->an_instance_member = 65;
}

int main(int argc, char *argv[]) {
    GTypeInfo my_type_info = {
        sizeof(myclass_t),
        
        NULL,
        NULL,

        (GClassInitFunc)my_class_init_func,
        NULL,
        NULL,

        sizeof(myinstance_t),
        0,
        (GInstanceInitFunc)my_instance_init_func,

        NULL,
    };

    GTypeFundamentalInfo my_fundamental_info = {
        G_TYPE_FLAG_CLASSED | G_TYPE_FLAG_INSTANTIATABLE
    };

    GType my_type_id = g_type_register_fundamental(
        g_type_fundamental_next(),
        "MyInstantiatableFundamentalType",
        &my_type_info,
        &my_fundamental_info,
        0
    );

    g_print("%d\n", my_type_id);
    g_print("%s\n", g_type_name(my_type_id));

    myinstance_t *instance = (myinstance_t*)g_type_create_instance(my_type_id);

    my_instance_method(instance, 78);
    my_class_method();

    g_print("Is instance of class %s? %s\n", g_type_name(my_type_id), G_TYPE_CHECK_INSTANCE_TYPE(instance, my_type_id) ? "yes" : "no");
    g_print("Is instance of class %s? %s\n", g_type_name(G_TYPE_INT), G_TYPE_CHECK_INSTANCE_TYPE(instance, G_TYPE_INT) ? "yes" : "no");

}

