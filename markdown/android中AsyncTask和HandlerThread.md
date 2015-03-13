#1. AsyncTask
    //这三个泛型分别代表“启动任务执行的输入参数”、
    //“后台任务执行的进度”、“后台计算结果的类型”
    class TestTask extends AsyncTask<String, Integer, Integer> {
        @Override
        protected void OnPreExecute() {
            /* 立即执行，一般用来在执行后台任务前更新UI */
        }
        
        @Override
        protected Integer doInBackground(String... params) {
            /* 在onPreExecute()完成后立即执行，用于执行较为费时的操作，此方法将接收输入参数和返回计算结果。在执行过程中可以调用publishProgress(Progress... values)来更新进度信息 */
        }
        
        @Override  
        protected void onProgressUpdate(Integer... values) {
            /* 将进度信息更新到UI组件 */
        }
        
        @Override  
        protected void onPostExecute(Integer result) {
            /* 后台操作结束时，此方法将会被调用 */
        }
    }
    
#2. Handler
##2.1 消息处理
    MyHandler mHander = new MyHandler();
    class MyHandler extends Handler {
        @Ovrride
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case XXX:
                    /*do somthing*/
                    break;
            }
        }
    }
##2.2 消息发送
    Message msg = mHander.obtainMessage();
    msg.what = XXX;
    //msg.obj = mObject;
    Bundle bundle = new Bundle();
    //...
    msg.setData(bundle);
    mHander.removeMessages(XXX);
    mHander.sendMessage(msg);

#3. HandlerThread
创建handler

    HandlerThread handlerThread = null;
    handlerThread = new HandlerThread("myTestThread");
    handlerThread.start();
    Handler handler = new Handler(handlerThread.getLooper()) {
        @Override
        public void handleMessage(Message msg) {
            //...
        }
    };
退出时可用.quit退出

    if (handlerThread!=null) {
        handlerThread.quit();
    }
