1. 取得ipc信息：
    ipcs [-m|-q|-s]
    -m      输出有关共享内存(shared memory)的信息
    -q      输出有关信息队列(message queue)的信息
    -s      输出有关“遮断器”(semaphore)的信息
2. 删除ipc
    ipcrm -m|-q|-s shm_id
    eg：%ipcrm -m 105
3.
 shmget(  )  创建一个新的共享内存区段
    取得一个共享内存区段的描述符
 shmctl(  )  取得一个共享内存区段的信息
    为一个共享内存区段设置特定的信息
    移除一个共享内存区段
 shmat(  )   挂接一个共享内存区段
 shmdt(  ) 于一个共享内存区段的分离
