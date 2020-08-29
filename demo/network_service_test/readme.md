#### 简述
chromium的发展越来越往微服务方向走，将各个模块拆分为一个个service，这些service既可以在同一个进程里，也可以分别在不同的进程里，通过Mojo的ipc构成了一个services网络，而Network Service就是其中的一个service
#### 启动
Network Service被设计为Mojo Service中的其中一个service，它不必知道自己是运行在哪个进程/线程。browser进程启动Network Service决定了它跑在browser进程(in-process)还是跑在utility进程(out-of-process)  
- out-of-process : Network Service运行在utility的IO线程上(content/utility/services.cc)  
- in-process : Network Service运行在browser进程的专属线程内,叫NetworkService线程(content/browser/network_service_instance_impl.cc)
#### 核心类
- NetworkService  
运行在单独的进程或者在单独的线程里，使用mojo与它进行通信，模糊了进程边界，可以把它看做一个单例，主要作用是用于网络请求，管理整个网络栈，browser进程持有它的一个Remote
- NetworkServiceClient  
它是NetworkService的一个client，运行在browser进程，NetworkService持有它的一个Remote，负责通知browser一些网络状态的改变
- NetworkContext  
由NetworkService创建，生活在NetworkService同一个进程或线程中，负责管理和创建URLRequestContext、CookieManager、HttpCache等等，也就是NetworkService的请求上下文，一般用它来创建URLLoaderFactory，只有特权进程才拥有使用NetworkContext的权利，因此由browser进程持有它的Remote
- URLLoaderFactory   
用于创建URLLoader的对象，一般情况下由NetworkContext创建，得益于mojo的设计，URLLoaderFactory的Remote由browser通过NetworkContext创建，然后把URLLoaderFactory的Remote传递给低权限的进程(renderer进程)，那么低权限的进程就能使用它创建URLLoader，然后持有一个URLLoader的Remote
- URLLoader   
实际去请求网络的类，它实际上内部是使用URLRequest进行请求，也就是net网络栈里提供的接口，以前chromium的低版本就是使用它进行网络请求的，只是现在我们接触不到这个类了，被URLLoader包裹起来了
- URLLoaderClient  
它就是接收URLLoader的response，实际上这个response就是mojo里面的DataPipe的一端，使用的是共享内存，renderder进程就能通过读这个pipe的一端获得数据