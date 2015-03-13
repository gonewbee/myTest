#1. List
##1.1 创建List
    List<String> myList = new ArrayList<String>();
    //or
    List<String> messages = Arrays.asList("Hello", "World", "How", "Are", "You");
##1.2 遍历List
    Iterator<String> it = myList.iterator();
    while (it.hasnext()) {
        String tmp = it.next();
        //do something...
    }
##1.3 Convert a Map to List
    //Map<Key, Value> map;假设有一个Map:
    Map<String, String> m = new HashMap<String, String>();
    m.put("Hello", "World");
    m.put("Apple", "3.14");
    m.put("Another", "Element");
    //由keys组成的List可由Set构建，这个Set从Map.keySet方法获取
    List<String> klist = new ArrayList<String>(m.keySet());
    //Map.values返回的是Collection
    List<String> vlist = new ArrayList<String>(m.values());
##1.4 Array to List
    int[] spam = new int[] {1, 2, 3};
    Arrays.asList(spam);
#2. Map
##2.1 遍历一个HashMap
Iterate through the entrySet like so:

    public static void printMap(Map mp) {
        Iterator it = mp.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry pairs = (Map.Entry)it.next();
            System.out.println(pairs.getKey() + " = " + pairs.getValue());
            it.remove(); // avoids a ConcurrentModificationException
        }
    }
#2.2 Map<Key, Value>根据value排序
[http://stackoverflow.com/questions/109383/how-to-sort-a-mapkey-value-on-the-values-in-java](http://stackoverflow.com/questions/109383/how-to-sort-a-mapkey-value-on-the-values-in-java)        
最简单的方法是使用TreeMap