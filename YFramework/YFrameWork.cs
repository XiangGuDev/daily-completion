
using System;
using System.Collections.Generic;
using System.Reflection;
#if UNITY_EDITOR
using UnityEditor;
#endif
using UnityEngine;

namespace YFrameWork
{

    #region BaseApp
    public interface ISetApp
    {
        void SetApp(IApp app);
    }

    /// <summary>
    /// app下的子模块接口
    /// </summary>
    public interface IModule
    {
        IApp GetApp();
    }

    /// <summary>
    /// app接口
    /// </summary>
    public interface IApp
    {
        /// <summary>
        /// 注册工具
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="model"></param>
        void RegisterUtility<T>(T model) where T : class, IUtility;

        /// <summary>
        /// 注册模型
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="model"></param>
        void RegisterModel<T>(T model) where T : class, IModel;

        /// <summary>
        /// 注册系统
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="system"></param>
        void RegisterSystem<T>(T system) where T : class, ISystem;

        /// <summary>
        /// 获取工具
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        T GetUtility<T>() where T : class, IUtility;

        /// <summary>
        /// 获取模型
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        T GetModel<T>() where T : class, IModel;

        /// <summary>
        /// 获取系统
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        T GetSystem<T>() where T : class, ISystem;

        /// <summary>
        /// 发送命令
        /// </summary>
        /// <typeparam name="T"></typeparam>
        void SendCommand<T>() where T : class, ICommand, new();

        /// <summary>
        /// 发送命令
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="command"></param>
        void SendCommand<T>(T command) where T : class, ICommand, new();

        /// <summary>
        /// 发送查询
        /// </summary>
        /// <typeparam name="TResult"></typeparam>
        /// <param name="query"></param>
        /// <returns></returns>
        TResult SendQuery<TResult>(IQuery<TResult> query);

        /// <summary>
        /// 发送事件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        void SendEvent<T>() where T : new();

        /// <summary>
        /// 发送事件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="e"></param>
        void SendEvent<T>(T e) where T : new();

        /// <summary>
        /// 注册事件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="onEvent"></param>
        /// <returns></returns>
        IUnRegister RegisterEvent<T>(Action<T> onEvent);

        /// <summary>
        /// 注销事件
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="onEvent"></param>
        void UnRegisterEvent<T>(Action<T> onEvent);
    }

    /// <summary>
    /// app基类
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public abstract class BaseApp<T> : BaseSingleton<T>, IApp
        where T : BaseApp<T>
    {
        protected BaseApp() { }

        abstract protected void InitApp();

        sealed public override void OnSingletonInit()
        {
            _bInit = false;

            // 初始化APP
            InitApp();

            OnInitApp?.Invoke(mInstance);

            // 初始化所有model
            for (int i = 0; i < _modelList.Count; i++)
            {
                _modelList[i].Init();
            }

            // 初始化所有system
            for (int i = 0; i < _systemList.Count; i++)
            {
                _systemList[i].Init();
            }

            _bInit = true;
        }

        public void RegisterUtility<TUtility>(TUtility storage) where TUtility : class, IUtility
        {
            mInstance._container.Register<TUtility>(storage);
        }

        public void RegisterModel<TModel>(TModel model) where TModel : class, IModel
        {
            mInstance._container.Register<TModel>(model);
            model.SetApp(this);
            if (_bInit)
            {
                model.Init();
            }
            else
            {
                _modelList.Add(model);
            }
        }

        public void RegisterSystem<TSystem>(TSystem system) where TSystem : class, ISystem
        {
            mInstance._container.Register<TSystem>(system);
            system.SetApp(this);
            if (_bInit)
            {
                system.Init();
            }
            else
            {
                _systemList.Add(system);
            }
        }

        public TUtility GetUtility<TUtility>() where TUtility : class, IUtility
        {
            return mInstance._container.Get<TUtility>();
        }

        public TModel GetModel<TModel>() where TModel : class, IModel
        {
            return mInstance._container.Get<TModel>();
        }

        public TSystem GetSystem<TSystem>() where TSystem : class, ISystem
        {
            return mInstance._container.Get<TSystem>();
        }

        public void SendCommand<TCommand>() where TCommand : class, ICommand, new()
        {
            var command = new TCommand();
            command.SetApp(this);
            command.Init();
            command.Execute();
        }

        public void SendCommand<TCommand>(TCommand command) where TCommand : class, ICommand, new()
        {
            command.SetApp(this);
            command.Init();
            command.Execute();
        }

        public void SendEvent<TEvent>() where TEvent : new()
        {
            _typeEventSystem.Send<TEvent>();
        }

        public void SendEvent<TEvent>(TEvent e) where TEvent : new()
        {
            _typeEventSystem.Send<TEvent>(e);
        }

        public IUnRegister RegisterEvent<TEvent>(Action<TEvent> onEvent)
        {
            return _typeEventSystem.Register<TEvent>(onEvent);
        }

        public void UnRegisterEvent<TEvent>(Action<TEvent> onEvent)
        {
            _typeEventSystem.UnRegister<TEvent>(onEvent);
        }

        public TResult SendQuery<TResult>(IQuery<TResult> query)
        {
            query.SetApp(this);
            return query.Do();
        }

        public static Action<T> OnInitApp = app => { };
        private ITypeEventSystem _typeEventSystem = new TypeEventSystem();
        private IOCContainer _container = new IOCContainer();
        private List<IModel> _modelList = new List<IModel>();
        private List<ISystem> _systemList = new List<ISystem>();
        private bool _bInit = false;
    }
    #endregion

    #region Controller
    /// <summary>
    /// 表现层接口
    /// </summary>
    public interface IController : IModule, ISendCommand, IGetSystem, IGetModel, IRegisterEvent, ISendQuery, ISendEvent
    {

    }
    #endregion

    #region System
    /// <summary>
    /// 系统层接口
    /// </summary>
    public interface ISystem : IModule, ISetApp, IGetModel, IGetUtility, IGetSystem, ISendEvent, IRegisterEvent
    {
        void Init();
    }

    public abstract class BaseSystem : ISystem
    {
        IApp IModule.GetApp()
        {
            return _app;
        }

        void ISystem.Init()
        {
            OnInit();
        }

        void ISetApp.SetApp(IApp app)
        {
            _app = app;
        }

        protected abstract void OnInit();

        private IApp _app;
    }
    #endregion

    #region Command
    /// <summary>
    /// 命令接口(用于关联模型和表现层)
    /// </summary>
    public interface ICommand : IModule, ISetApp, ISendQuery, IGetSystem, IGetModel, IGetUtility, ISendEvent, ISendCommand
    {
        void Init();
        void Execute();
    }

    public abstract class BaseCommand : ICommand
    {
        void ICommand.Execute()
        {
            OnExecute();
        }

        void ICommand.Init()
        {
            OnInit();
        }

        IApp IModule.GetApp()
        {
            return _app;
        }

        void ISetApp.SetApp(IApp app)
        {
            _app = app;
        }

        protected abstract void OnExecute();
        protected abstract void OnInit();

        private IApp _app;
    }
    #endregion

    #region  Model
    /// <summary>
    /// 模型接口
    /// </summary>
    public interface IModel : IModule, ISetApp, IGetUtility, ISendEvent
    {
        void Init();
    }

    public abstract class BaseModel : IModel
    {
        IApp IModule.GetApp()
        {
            return _app;
        }

        void IModel.Init()
        {
            OnInit();
        }

        protected abstract void OnInit();

        void ISetApp.SetApp(IApp app)
        {
            _app = app;
        }

        private IApp _app;
    }
    #endregion

    #region Query
    public interface IQuery<TResult> : IModule, ISendQuery, ISetApp, IGetModel, IGetSystem
    {
        TResult Do();
    }

    public abstract class BaseQuery<T> : IQuery<T>
    {
        public T Do()
        {
            return OnDo();
        }

        abstract protected T OnDo();

        public IApp GetApp()
        {
            return _app;
        }

        public void SetApp(IApp app)
        {
            _app = app;
        }

        private IApp _app;
    }

    #endregion

    #region Utility
    public interface IUtility
    {

    }
    /// <summary>
    /// 存储类接口
    /// </summary>
    public interface IStorage : IUtility
    {
        void SaveString(string key, string val);
        string LoadString(string key, string defVal = "");

        void SaveInt(string key, int val);

        int LoadInt(string key, int defVal = 0);
    }

    public class PlayerPrefsStorage : IStorage
    {
        public void SaveString(string key, string val)
        {
            PlayerPrefs.SetString(key, val);
        }

        public string LoadString(string key, string defVal = "")
        {
            return PlayerPrefs.GetString(key, defVal);
        }


        public void SaveInt(string key, int val)
        {
            //Debug.Log("PlayerPrefs.SetInt : " + key + " / " + val);
            PlayerPrefs.SetInt(key, val);
        }

        public int LoadInt(string key, int defVal = 0)
        {
            //Debug.Log("PlayerPrefs.LoadInt : " + key + " / " + PlayerPrefs.GetInt(key, defVal));
            return PlayerPrefs.GetInt(key, defVal);
        }
    }

    public class EditorPrefsStorage : IStorage
    {
        public void SaveString(string key, string val)
        {
#if UNITY_EDITOR
            EditorPrefs.SetString(key, val);
#endif
        }

        public string LoadString(string key, string defVal = "")
        {
#if UNITY_EDITOR
            return EditorPrefs.GetString(key, defVal);
#else
            return "";
#endif
        }

        public void SaveInt(string key, int val)
        {
#if UNITY_EDITOR
            //Debug.Log("EditorPrefs.SetInt : " + key + " / " + val);
            EditorPrefs.SetInt(key, val);
#endif
        }

        public int LoadInt(string key, int defVal = 0)
        {
#if UNITY_EDITOR
            //Debug.Log("EditorPrefs.LoadInt : " + key + " / " + EditorPrefs.GetInt(key, defVal));
            return EditorPrefs.GetInt(key, defVal);
#else
            return 0;
#endif
        }
    }
    #endregion

    #region Rule
    /// <summary>
    /// 获取系统接口
    /// </summary>
    public interface IGetSystem : IModule
    {

    }

    public static class CanGetSystemEx
    {
        public static T GetSystem<T>(this IGetSystem self) where T : class, ISystem
        {
            return self.GetApp().GetSystem<T>();
        }
    }
    /// <summary>
    /// 获取模型接口
    /// </summary>
    public interface IGetModel : IModule
    {

    }

    public static class CanGetModelEx
    {
        public static T GetModel<T>(this IGetModel self) where T : class, IModel
        {
            return self.GetApp().GetModel<T>();
        }
    }
    /// <summary>
    /// 获取工具接口
    /// </summary>
    public interface IGetUtility : IModule
    {

    }

    public static class CanGetUtilityEx
    {
        public static T GetUtility<T>(this IGetUtility self) where T : class, IUtility
        {
            return self.GetApp().GetUtility<T>();
        }
    }
    /// <summary>
    /// 发送命令接口
    /// </summary>
    public interface ISendCommand : IModule
    {

    }

    public static class CanSendCommandEx
    {
        public static void SendCommand<T>(this ISendCommand self) where T : class, ICommand, new()
        {
            self.GetApp().SendCommand<T>();
        }

        public static void SendCommand<T>(this ISendCommand self, T cmd) where T : class, ICommand, new()
        {
            self.GetApp().SendCommand<T>(cmd);
        }
    }
    /// <summary>
    /// 注册事件接口
    /// </summary>
    public interface IRegisterEvent : IModule
    {

    }

    public static class CanRegisterEventEx
    {
        public static IUnRegister RegisterEvent<T>(this IRegisterEvent self, Action<T> onEvent)
        {
            return self.GetApp().RegisterEvent<T>(onEvent);
        }

        public static void UnRegisterEvent<T>(this IRegisterEvent self, Action<T> onEvent)
        {
            self.GetApp().UnRegisterEvent<T>(onEvent);
        }
    }
    /// <summary>
    /// 发送事件接口
    /// </summary>
    public interface ISendEvent : IModule
    {

    }

    public static class CanSendEventEx
    {
        public static void SendEvent<T>(this ISendEvent self) where T : new()
        {
            self.GetApp().SendEvent<T>();
        }

        public static void SendEvent<T>(this ISendEvent self, T e) where T : new()
        {
            self.GetApp().SendEvent<T>(e);
        }
    }
    /// <summary>
    /// 获取模型接口
    /// </summary>
    public interface ISendQuery : IModule
    {

    }

    public static class CanSendQueryEx
    {
        public static TResult SendQuery<TResult>(this ISendQuery self, IQuery<TResult> query)
        {
            return self.GetApp().SendQuery<TResult>(query);
        }
    }
    #endregion

    #region TypeEventSystem
    public interface IUnRegister
    {
        void UnRegister();
    }

    public class TypeEventSystemUnRegister<T> : IUnRegister
    {
        public void UnRegister()
        {
            _typeEventSystem.UnRegister<T>(_onEvent);

            _typeEventSystem = null;
            _onEvent = null;
        }

        public ITypeEventSystem _typeEventSystem;
        public Action<T> _onEvent;
    }

    public interface ITypeEventSystem
    {
        void Send<T>() where T : new();
        void Send<T>(T e);
        IUnRegister Register<T>(Action<T> onEvent);
        void UnRegister<T>(Action<T> onEvent);
    }

    public class TypeEventSystem : ITypeEventSystem
    {
        public interface IEventRegs
        {

        }

        public class EventRegs<T> : IEventRegs
        {
            public Action<T> _onEvent = e => { };
        }

        public void Send<T>() where T : new()
        {
            var e = new T();
            Send<T>(e);
        }

        public void Send<T>(T e)
        {
            var type = typeof(T);
            IEventRegs regs;
            if (_eventRegList.TryGetValue(type, out regs))
            {
                (regs as EventRegs<T>)._onEvent(e);
            }
        }

        public IUnRegister Register<T>(Action<T> onEvent)
        {
            var type = typeof(T);
            IEventRegs regs;
            if (_eventRegList.TryGetValue(type, out regs))
            {

            }
            else
            {
                regs = new EventRegs<T>();
                _eventRegList.Add(type, regs);
            }
            (regs as EventRegs<T>)._onEvent += onEvent;

            return new TypeEventSystemUnRegister<T>()
            {
                _onEvent = onEvent,
                _typeEventSystem = this
            };
        }

        public void UnRegister<T>(Action<T> onEvent)
        {
            var type = typeof(T);
            IEventRegs regs;
            if (_eventRegList.TryGetValue(type, out regs))
            {
                (regs as EventRegs<T>)._onEvent -= onEvent;

            }
        }

        public static readonly TypeEventSystem Global = new TypeEventSystem();
        Dictionary<Type, IEventRegs> _eventRegList = new Dictionary<Type, IEventRegs>();
    }

    public interface IOnEvent<T>
    {
        void OnEvent(T e);
    }

    public static class OnGlobalEventEx
    {
        public static IUnRegister RegisterEvent<T>(this IOnEvent<T> self) where T : struct
        {
            return TypeEventSystem.Global.Register<T>(self.OnEvent);
        }

        public static void UnRegisterEvent<T>(this IOnEvent<T> self) where T : struct
        {
            TypeEventSystem.Global.UnRegister<T>(self.OnEvent);
        }
    }

    public class UnRegisterOnDestroyTrigger : MonoBehaviour
    {
        public void AddUnRegister(IUnRegister unReg)
        {
            _unregList.Add(unReg);
        }

        private void OnDestroy()
        {
            foreach (var unReg in _unregList)
            {
                unReg.UnRegister();
            }
            _unregList.Clear();
        }

        private HashSet<IUnRegister> _unregList = new HashSet<IUnRegister>();
    }

    public static class UnRegisterEx
    {
        public static void UnRegisterOnDestroy(this IUnRegister self, GameObject obj)
        {
            var trigger = obj.GetComponent<UnRegisterOnDestroyTrigger>();
            if (trigger == null)
            {
                trigger = obj.AddComponent<UnRegisterOnDestroyTrigger>();
            }

            trigger.AddUnRegister(self);
        }
    }
    #endregion

    #region IOC
    /// <summary>
    /// IOC容器类（本质上是一个字典）
    /// </summary>
    public class IOCContainer
    {
        public void Register<T>(T instance)
        {
            Type key = typeof(T);
            if (_instances.ContainsKey(key))
            {
                _instances[key] = instance;
            }
            else
            {
                _instances.Add(key, instance);
            }
        }

        public T Get<T>() where T : class
        {
            var key = typeof(T);
            object retObj;
            if (_instances.TryGetValue(key, out retObj))
            {
                return retObj as T;
            }
            return null;
        }

        private Dictionary<Type, object> _instances = new Dictionary<Type, object>();
    }
    #endregion

    #region BindableProperty
    /// <summary>
    /// 属性封装类
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class BindableProperty<T>
    {
        public BindableProperty(T defVal = default)
        {
            _value = defVal;
        }

        public T Value
        {
            get
            {
                return _value;
            }

            set
            {
                if (value == null || !value.Equals(_value))
                {
                    _value = value;
                    _onValueChanged?.Invoke(value);
                }
            }
        }

        public IUnRegister Register(Action<T> onValueChanged)
        {
            _onValueChanged += onValueChanged;
            return new BindablePropertyUnRegister<T>()
            {
                _bindableProperty = this,
                _onValueChanged = onValueChanged
            };
        }

        public IUnRegister RegisterByDef(Action<T> onValueChanged)
        {
            onValueChanged(_value);
            return Register(onValueChanged);
        }

        public void UnRegister(Action<T> onValueChanged)
        {
            _onValueChanged -= onValueChanged;
        }

        public static implicit operator T(BindableProperty<T> property)
        {
            return property.Value;
        }

        public override string ToString()
        {
            return Value.ToString();
        }

        private Action<T> _onValueChanged = (v) => { };
        private T _value;
    }

    public class BindablePropertyUnRegister<T> : IUnRegister
    {
        public void UnRegister()
        {
            _bindableProperty.UnRegister(_onValueChanged);
        }

        public BindableProperty<T> _bindableProperty { get; set; }
        public Action<T> _onValueChanged { get; set; }
    }
    #endregion

    #region Singleton
    /// <summary>
    /// 单例接口
    /// </summary>
    public interface ISingleton
    {
        /// <summary>
        /// 单例初始化(继承当前接口的类都需要实现该方法)
        /// </summary>
        void OnSingletonInit();
    }

    /// <summary>
    /// 普通类的单例
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public abstract class BaseSingleton<T> : ISingleton where T : BaseSingleton<T>
    {
        /// <summary>
        /// 静态实例
        /// </summary>
        protected static T mInstance;

        /// <summary>
        /// 标签锁：确保当一个线程位于代码的临界区时，另一个线程不进入临界区。
        /// 如果其他线程试图进入锁定的代码，则它将一直等待（即被阻止），直到该对象被释放
        /// </summary>
        static object mLock = new object();

        /// <summary>
        /// 静态属性
        /// </summary>
        public static T Instance
        {
            get
            {
                lock (mLock)
                {
                    if (mInstance == null)
                    {
                        mInstance = SingletonCreator.CreateSingleton<T>();
                        mInstance.OnSingletonInit();
                    }
                }

                return mInstance;
            }
        }

        /// <summary>
        /// 资源释放
        /// </summary>
        public virtual void Dispose()
        {
            mInstance = null;
        }

        /// <summary>
        /// 单例初始化方法
        /// </summary>
        public virtual void OnSingletonInit()
        {
        }
    }

    /// <summary>
    /// 属性单例类
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public static class SingletonProperty<T> where T : class, ISingleton
    {
        /// <summary>
        /// 静态实例
        /// </summary>
        private static T mInstance;

        /// <summary>
        /// 标签锁
        /// </summary>
        private static readonly object mLock = new object();

        /// <summary>
        /// 静态属性
        /// </summary>
        public static T Instance
        {
            get
            {
                lock (mLock)
                {
                    if (mInstance == null)
                    {
                        mInstance = SingletonCreator.CreateSingleton<T>();
                        mInstance.OnSingletonInit();
                    }
                }

                return mInstance;
            }
        }

        /// <summary>
        /// 资源释放
        /// </summary>
        public static void Dispose()
        {
            mInstance = null;
        }
    }

    /// <summary>
    /// 普通单例创建类
    /// </summary>
    internal static class SingletonCreator
    {
        static T CreateNonPublicConstructorObject<T>() where T : class
        {
            var type = typeof(T);
            // 获取私有构造函数
            var constructorInfos = type.GetConstructors(BindingFlags.Instance | BindingFlags.NonPublic);

            // 获取无参构造函数
            var ctor = Array.Find(constructorInfos, c => c.GetParameters().Length == 0);

            if (ctor == null)
            {
                throw new Exception("Non-Public Constructor() not found! in " + type);
            }

            return ctor.Invoke(null) as T;
        }

        public static T CreateSingleton<T>() where T : class, ISingleton
        {
            var type = typeof(T);
            var monoBehaviourType = typeof(MonoBehaviour);

            if (monoBehaviourType.IsAssignableFrom(type))
            {
                return CreateMonoSingleton<T>();
            }
            else
            {
                var instance = CreateNonPublicConstructorObject<T>();
                return instance;
            }
        }


        /// <summary>
        /// 单元测试模式 标签
        /// </summary>
        public static bool IsUnitTestMode { get; set; }

        /// <summary>
        /// 查找Obj（一个嵌套查找Obj的过程）
        /// </summary>
        /// <param name="root">父节点</param>
        /// <param name="subPath">拆分后的路径节点</param>
        /// <param name="index">下标</param>
        /// <param name="build">true</param>
        /// <param name="dontDestroy">不要销毁 标签</param>
        /// <returns></returns>
        private static GameObject FindGameObject(GameObject root, string[] subPath, int index, bool build,
            bool dontDestroy)
        {
            GameObject client = null;

            if (root == null)
            {
                client = GameObject.Find(subPath[index]);
            }
            else
            {
                var child = root.transform.Find(subPath[index]);
                if (child != null)
                {
                    client = child.gameObject;
                }
            }

            if (client == null)
            {
                if (build)
                {
                    client = new GameObject(subPath[index]);
                    if (root != null)
                    {
                        client.transform.SetParent(root.transform);
                    }

                    if (dontDestroy && index == 0 && !IsUnitTestMode)
                    {
                        GameObject.DontDestroyOnLoad(client);
                    }
                }
            }

            if (client == null)
            {
                return null;
            }

            return ++index == subPath.Length ? client : FindGameObject(client, subPath, index, build, dontDestroy);
        }

        /// <summary>
        /// 泛型方法：创建MonoBehaviour单例
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <returns></returns>
        public static T CreateMonoSingleton<T>() where T : class, ISingleton
        {
            T instance = null;
            var type = typeof(T);

            //判断T实例存在的条件是否满足
            if (!IsUnitTestMode && !Application.isPlaying)
                return instance;

            //判断当前场景中是否存在T实例
            instance = UnityEngine.Object.FindObjectOfType(type) as T;
            if (instance != null)
            {
                instance.OnSingletonInit();
                return instance;
            }

            //MemberInfo：获取有关成员属性的信息并提供对成员元数据的访问
            MemberInfo info = typeof(T);
            //获取T类型 自定义属性，并找到相关路径属性，利用该属性创建T实例
            var attributes = info.GetCustomAttributes(true);
            foreach (var atribute in attributes)
            {
                var defineAttri = atribute as MonoSingletonPath;
                if (defineAttri == null)
                {
                    continue;
                }

                instance = CreateComponentOnGameObject<T>(defineAttri.PathInHierarchy, true);
                break;
            }

            //如果还是无法找到instance  则主动去创建同名Obj 并挂载相关脚本 组件
            if (instance == null)
            {
                var obj = new GameObject(typeof(T).Name);
                if (!IsUnitTestMode)
                    UnityEngine.Object.DontDestroyOnLoad(obj);
                instance = obj.AddComponent(typeof(T)) as T;
            }

            return instance;
        }

        /// <summary>
        /// 在GameObject上创建T组件（脚本）
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="path">路径（应该就是Hierarchy下的树结构路径）</param>
        /// <param name="dontDestroy">不要销毁 标签</param>
        /// <returns></returns>
        private static T CreateComponentOnGameObject<T>(string path, bool dontDestroy) where T : class
        {
            var obj = FindGameObject(path, true, dontDestroy);
            if (obj == null)
            {
                obj = new GameObject("Singleton of " + typeof(T).Name);
                if (dontDestroy && !IsUnitTestMode)
                {
                    UnityEngine.Object.DontDestroyOnLoad(obj);
                }
            }

            return obj.AddComponent(typeof(T)) as T;
        }

        /// <summary>
        /// 查找Obj（对于路径 进行拆分）
        /// </summary>
        /// <param name="path">路径</param>
        /// <param name="build">true</param>
        /// <param name="dontDestroy">不要销毁 标签</param>
        /// <returns></returns>
        private static GameObject FindGameObject(string path, bool build, bool dontDestroy)
        {
            if (string.IsNullOrEmpty(path))
            {
                return null;
            }

            var subPath = path.Split('/');
            if (subPath == null || subPath.Length == 0)
            {
                return null;
            }

            return FindGameObject(null, subPath, 0, build, dontDestroy);
        }
    }

    /// <summary>
    /// 静态类：MonoBehaviour类的单例
    /// 泛型类：Where约束表示T类型必须继承MonoSingleton<T>
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public abstract class MonoSingleton<T> : MonoBehaviour, ISingleton where T : MonoSingleton<T>
    {
        /// <summary>
        /// 静态实例
        /// </summary>
        protected static T mInstance;

        /// <summary>
        /// 静态属性：封装相关实例对象
        /// </summary>
        public static T Instance
        {
            get
            {
                if (mInstance == null && !mOnApplicationQuit)
                {
                    mInstance = SingletonCreator.CreateMonoSingleton<T>();
                    mInstance.OnSingletonInit();
                }

                return mInstance;
            }
        }

        /// <summary>
        /// 实现接口的单例初始化
        /// </summary>
        public virtual void OnSingletonInit()
        {
        }

        /// <summary>
        /// 资源释放
        /// </summary>
        public virtual void Dispose()
        {
            if (SingletonCreator.IsUnitTestMode)
            {
                var curTrans = transform;
                do
                {
                    var parent = curTrans.parent;
                    DestroyImmediate(curTrans.gameObject);
                    curTrans = parent;
                } while (curTrans != null);

                mInstance = null;
            }
            else
            {
                Destroy(gameObject);
            }
        }

        /// <summary>
        /// 当前应用程序是否结束 标签
        /// </summary>
        protected static bool mOnApplicationQuit = false;

        /// <summary>
        /// 应用程序退出：释放当前对象并销毁相关GameObject
        /// </summary>
        protected virtual void OnApplicationQuit()
        {
            mOnApplicationQuit = true;
            if (mInstance == null) return;
            Destroy(mInstance.gameObject);
            mInstance = null;
        }

        /// <summary>
        /// 释放当前对象
        /// </summary>
        protected virtual void OnDestroy()
        {
            mInstance = null;
        }

        /// <summary>
        /// 判断当前应用程序是否退出
        /// </summary>
        public static bool IsApplicationQuit
        {
            get { return mOnApplicationQuit; }
        }
    }

    /// <summary>
    /// MonoSingleton路径
    /// </summary>
    [AttributeUsage(AttributeTargets.Class)] //这个特性只能标记在Class上
    public class MonoSingletonPath : Attribute
    {
        private string mPathInHierarchy;

        public MonoSingletonPath(string pathInHierarchy)
        {
            mPathInHierarchy = pathInHierarchy;
        }

        public string PathInHierarchy
        {
            get { return mPathInHierarchy; }
        }
    }

    /// <summary>
    /// 继承Mono的属性单例？
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public static class MonoSingletonProperty<T> where T : MonoBehaviour, ISingleton
    {
        private static T mInstance;

        public static T Instance
        {
            get
            {
                if (null == mInstance)
                {
                    mInstance = SingletonCreator.CreateMonoSingleton<T>();
                    mInstance.OnSingletonInit();
                }

                return mInstance;
            }
        }

        public static void Dispose()
        {
            if (SingletonCreator.IsUnitTestMode)
            {
                UnityEngine.Object.DestroyImmediate(mInstance.gameObject);
            }
            else
            {
                UnityEngine.Object.Destroy(mInstance.gameObject);
            }

            mInstance = null;
        }
    }

    /// <summary>
    /// 如果跳转到新的场景里已经有了实例，则不创建新的单例（或者创建新的单例后会销毁掉新的单例）
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public abstract class PersistentMonoSingleton<T> : MonoBehaviour where T : Component
    {
        protected static T mInstance;
        protected bool mEnabled;

        /// <summary>
        /// Singleton design pattern
        /// </summary>
        /// <value>The instance.</value>
        public static T Instance
        {
            get
            {
                if (mInstance == null)
                {
                    mInstance = FindObjectOfType<T>();
                    if (mInstance == null)
                    {
                        var obj = new GameObject();
                        mInstance = obj.AddComponent<T>();
                    }
                }

                return mInstance;
            }
        }

        /// <summary>
        /// On awake, we check if there's already a copy of the object in the scene. If there's one, we destroy it.
        /// </summary>
        protected virtual void Awake()
        {
            if (!Application.isPlaying)
            {
                return;
            }

            if (mInstance == null)
            {
                //If I am the first instance, make me the Singleton
                mInstance = this as T;
                DontDestroyOnLoad(transform.gameObject);
                mEnabled = true;
            }
            else
            {
                //If a Singleton already exists and you find
                //another reference in scene, destroy it!
                if (this != mInstance)
                {
                    Destroy(this.gameObject);
                }
            }
        }
    }

    /// <summary>
    /// 如果跳转到新的场景里已经有了实例，则删除已有示例，再创建新的实例
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class ReplaceableMonoSingleton<T> : MonoBehaviour where T : Component
    {
        protected static T mInstance;
        public float InitializationTime;

        /// <summary>
        /// Singleton design pattern
        /// </summary>
        /// <value>The instance.</value>
        public static T Instance
        {
            get
            {
                if (mInstance == null)
                {
                    mInstance = FindObjectOfType<T>();
                    if (mInstance == null)
                    {
                        GameObject obj = new GameObject();
                        obj.hideFlags = HideFlags.HideAndDontSave;
                        mInstance = obj.AddComponent<T>();
                    }
                }

                return mInstance;
            }
        }

        /// <summary>
        /// On awake, we check if there's already a copy of the object in the scene. If there's one, we destroy it.
        /// </summary>
        protected virtual void Awake()
        {
            if (!Application.isPlaying)
            {
                return;
            }

            InitializationTime = Time.time;

            DontDestroyOnLoad(this.gameObject);
            // we check for existing objects of the same type
            T[] check = FindObjectsOfType<T>();
            foreach (T searched in check)
            {
                if (searched != this)
                {
                    // if we find another object of the same type (not this), and if it's older than our current object, we destroy it.
                    if (searched.GetComponent<ReplaceableMonoSingleton<T>>().InitializationTime < InitializationTime)
                    {
                        Destroy(searched.gameObject);
                    }
                }
            }

            if (mInstance == null)
            {
                mInstance = this as T;
            }
        }
    }
    #endregion
}