# 设计模式

### 1. 创建型模式

- 简单工厂(Simple Factory)
    
    把对象的创建封装在一个接口函数里面，通过传入不同的标识，返回创建的对象。

    客户不用自己负责new对象，不用了解对象创建的详细过程。

    提供方创建对象实例的接口函数不闭合，不能对修改闭合。

- 工厂方法(Factory Method)

    Factory基类，提供了一个纯虚函数(创建产品)，定义派生类(具体产品的工厂)负责创建对应的产品，可以做到不同的产品在不同的工厂里创建，能够对现有工厂，以及产品的修改关闭。

    实际上，很多产品是有关联关系的，属于一个产品簇，不应该放在不同的工厂里面创建，这样一是不符合实际的产品对象创建逻辑，二是工厂类太多了，不好维护。

- 抽象工厂(Abstract Factory)

    把有关联关系的，属于一个产品簇的所有产品创建的接口函数，放在一个抽象工厂里面AbstractFactory，派生类(具体产品的工厂)应该复杂创建该产品簇里面所有的产品。

    ```
    // 在一支球队中，中场和后卫是有关联关系的
    class Midfielder { public: virtual void show() = 0; };
    class Iniesta : public Midfielder { public: void show() { cout << "1" << endl; } };

    class Defender{ public: virtual void show() = 0; };
    class Cannavaro : public Defender { public: void show() { cout << "2" << endl; } };

    // 抽象工厂
    class AbstractFactroy
    {
    public:
        virtual Midfielder* createMidfielder() = 0;
        virtual Defender* createDefender() = 0;
    };

    // 球队工厂
    class TeamFactory: public AbstractFactroy
    {
    public:
        Midfielder* createMidfielder()
        {
            return new Iniesta();
        }
        Defender* createDefender()
        {
            return new Cannavaro();
        }
    };
    ```
    ```
    unique_ptr<AbstractFactroy> teamf(new TeamFactory());
    teamf->createMidfielder()->show();
    teamf->createDefender()->show();
    ```