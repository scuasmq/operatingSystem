```c
rmutex,wmutex:semaphore
rmutex :=1;wmutex :=1;
rcount:int
rcount:=0;
	//读者R									//写者W
  Wait(rmutex);						Wait(wmutex);
  rcount++;								Write_Action();
  if(rcount == 1)					Signal(wmutex);
  Wait(wmutex);
  Signal(rmutex);
  Read_Action();
  Wait(rmutex);
  rcount--;
  if(rcount == 0)
  Signal(wmutex);
  Signal(rmutex);


```

