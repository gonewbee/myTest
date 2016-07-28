/**
* @brief 测试GObject的property功能
* 
* 使用gcc -E可将宏扩展，可看到G_DEFINE_TYPE自动实现 xx_get_type功能
* gcc -E class_properties_7.c `pkg-config --cflags --libs gobject-2.0 glib-2.0`
*/
#include <glib.h>
#include <glib-object.h>

enum {
    PROP_0,
    PROP_NAME,
    PROP_NUMBER
};

/**
* 实例结构体
*/
typedef struct {
    GObject parent;

    /* 私有变量 */
    char *private_name;
    int private_num;
} ZsyTest;

/**
* 对象结构体
*/
typedef struct {
    GObjectClass parent_class;
} ZsyTestClass;

GType zsy_test_get_type(void);

#define ZSY_TYPE_TEST               (zsy_test_get_type())
#define ZSY_TEST(obj)               (G_TYPE_CHECK_INSTANCE_CAST((obj), ZSY_TYPE_TEST, ZsyTest))
#define ZSY_TEST_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST((klass), ZSY_TYPE_TEST, ZsyTestClass))
#define ZSY_IS_TEST(obj)            (G_TYPE_CHECK_INSTANCE_TYPE((obj), ZSY_TYPE_TEST))
#define ZSY_IS_TEST_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE((klass), ZSY_TYPE_TEST))
#define ZSY_TEST_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS((obj), ZSY_TYPE_TEST, ZsyTestClass))

#if 1
/* G_DEFINE_TYPE宏可是实现zsy_test_get_type */
G_DEFINE_TYPE(ZsyTest, zsy_test, G_TYPE_OBJECT)
#endif

static void test_get_property(GObject *object,
    guint prop_id, GValue *value, GParamSpec *pspec) {
    ZsyTest *instance = ZSY_TEST(object);
    switch(prop_id) {
        case PROP_NAME:
            g_value_set_string(value, instance->private_name);
            break;
        case PROP_NUMBER:
            g_value_set_int(value, instance->private_num);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

static void test_set_property(GObject *object, 
    guint prop_id, const GValue *value, GParamSpec *pspec) {
    ZsyTest *instance = ZSY_TEST(object);
    switch(prop_id) {
        case PROP_NAME:
            g_free(instance->private_name);
            instance->private_name = g_value_dup_string(value);
            break;
        case PROP_NUMBER:
            instance->private_num = g_value_get_int(value);
            // g_print("%s set number %d\n", __func__, instance->private_num);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
            break;
    }
}

/**
* @brief 对象初始化函数
*/
static void zsy_test_init(ZsyTest *instance) {
    g_print("%s called!\n", __func__);
}

/**
* @brief 类初始化函数
*/
static void zsy_test_class_init(ZsyTestClass *klass) {
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->get_property = test_get_property;
    gobject_class->set_property = test_set_property;

    g_object_class_install_property(gobject_class, PROP_NAME,
            g_param_spec_string("name",
                                "Name property",
                                "The name property of my class. Can be used on constructing.",
                                "All your property are belong to us.",
                                G_PARAM_READABLE|G_PARAM_WRITABLE|G_PARAM_CONSTRUCT));
    g_object_class_install_property(gobject_class, PROP_NUMBER,
            g_param_spec_int("number",
                             "Number property",
                             "The number property of my class.",
                             G_MININT,
                             G_MAXINT,
                             42,
                             G_PARAM_READABLE|G_PARAM_WRITABLE));

    int signal_id = g_signal_new(
                    "my-signal",
                    G_OBJECT_CLASS_TYPE(gobject_class),
                    G_SIGNAL_RUN_FIRST,
                    0,
                    NULL,
                    NULL,
                    g_cclosure_marshal_VOID__STRING,
                    G_TYPE_NONE,
                    1,
                    G_TYPE_STRING,
                    NULL);

    g_signal_new(
            "my-signal-int",
            G_OBJECT_CLASS_TYPE(gobject_class),
            G_SIGNAL_RUN_FIRST,
            0,
            NULL,
            NULL,
            g_cclosure_marshal_VOID__INT,
            G_TYPE_NONE,
            1,
            G_TYPE_INT,
            NULL);
}

#if 0
GType zsy_test_get_type() {
    static int zsy_test_id = 0;
    if (zsy_test_id==0) {
        GTypeInfo my_type_info = {
            sizeof(ZsyTestClass),
            NULL,
            NULL,

            (GClassInitFunc)zsy_test_class_init,
            NULL,
            NULL,

            sizeof(ZsyTest),
            0,
            (GInstanceInitFunc)zsy_test_init,

            NULL,
        };
        zsy_test_id = g_type_register_static(G_TYPE_OBJECT,
                    "ZsyTest",
                    &my_type_info,
                    0);
    }
    return zsy_test_id;
}
#endif

static void hello(GObject *sender, char *name, gpointer data) {
    g_print("Hello World! %s\n", name);
}

static void goodbye(GObject *sender, char *name, gpointer data) {
    g_print("Goodbye %s, data is [%d]\n", name, *(int*)data);
}

static void hello_int(GObject *sender, int d, gpointer data) {
    g_print("%s called value:%d\n", __func__, d);
}

int main(int argc, char *argv[]) {
    g_print("Type id:%d\n", ZSY_TYPE_TEST);
    g_print("Type name:%s\n", g_type_name(ZSY_TYPE_TEST));

    GValue *value = g_new0(GValue, 1);

    // 实例对象
    ZsyTest *instance = (ZsyTest*)g_object_new(ZSY_TYPE_TEST, NULL);
    // 获取对象属性值
    g_value_init(value, G_TYPE_STRING);
    g_object_get_property(G_OBJECT(instance), "name", value);
    g_print("Property name: %s\n", g_value_get_string(value));
    g_value_unset(value);

    g_value_init(value, G_TYPE_INT);
    g_object_get_property(G_OBJECT(instance), "number", value);
    g_print("Property number: %d\n", g_value_get_int(value));
    g_value_unset(value);

    g_object_unref(instance);


    // 实例对象
    instance = (ZsyTest*)g_object_new(ZSY_TYPE_TEST,
            "name", "Hello",
            "number", 75,
            NULL);
    g_value_init(value, G_TYPE_STRING);
    g_object_get_property(G_OBJECT(instance), "name", value);
    g_print("Property name: %s\n", g_value_get_string(value));
    g_value_unset(value);

    g_value_init(value, G_TYPE_INT);
    g_object_get_property(G_OBJECT(instance), "number", value);
    g_print("Property number: %d\n", g_value_get_int(value));
    g_value_unset(value);

    g_value_init(value, G_TYPE_INT);
    g_value_set_int(value, 81);
    g_object_set_property(G_OBJECT(instance), "number", value);
    g_value_unset(value);


    g_value_init(value, G_TYPE_INT);
    g_object_get_property(G_OBJECT(instance), "number", value);
    g_print("Property number: %d\n", g_value_get_int(value));
    g_value_unset(value);

    int tmpvar = 42;
    g_signal_connect(instance, "my-signal", G_CALLBACK(hello), NULL);
    g_signal_connect(instance, "my-signal", G_CALLBACK(goodbye), &tmpvar);
    g_signal_emit_by_name(instance, "my-signal", "Test");

    g_signal_connect(instance, "my-signal-int", G_CALLBACK(hello_int), NULL);
    g_signal_emit_by_name(instance, "my-signal-int", tmpvar);

    g_object_unref(instance);
    g_free(value);
    return 0;
}



