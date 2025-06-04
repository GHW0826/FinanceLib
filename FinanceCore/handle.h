#pragma once

/* 
	관찰 가능한 항목에 대한 공유 핸들
	클래스의 모든 인스턴스 사본은 동일한 것을 참조
	재연결 가능한 스마트 포인터를 통해 관찰할 수 있다. 
	그런 경우 포인터가 다른 관측값에 다시 연결되면 복사된다.
	T는 Observable에서 상속되어야 함
 */
template <class T>
class Handle 
{
protected:
    class Link : public Observable, public Observer {
    public:
        Link(const SharedPtr<T>& h, bool registerAsObserver);
        Link(SharedPtr<T>&& h, bool registerAsObserver);
        void LinkTo(SharedPtr<T>, bool registerAsObserver);
        bool Empty() const { return !_h; }
        const SharedPtr<T>& CurrentLink() const { return _h; }
        void Update() override { notifyObservers(); }

    private:
        SharedPtr<T> _h;
        bool _isObserver = false;
    };
protected:
    SharedPtr<Link> _link;
};

// 옵저버블(Observable) 객체에 대한 재연결 가능한 핸들
/*
    이 클래스의 인스턴스는 다른 옵저버블 객체를 가리키도록 재연결(relink)할 수 있다.
    이렇게 재연결되면, 해당 인스턴스를 복사하여 생성된 모든 핸들에도 변경이 전파됨

    pre 클래스 T는 Observable을 상속받아야 함.

    \warning `Handle` 문서를 참고하여 <tt>registerAsObserver</tt>와 관련된 이슈에 유의..
*/
template <class T>
class RelinkableHandle : public Handle<T> {
public:
    RelinkableHandle()
        : RelinkableHandle(SharedPtr<T>()) 
    {}
    explicit RelinkableHandle(const SharedPtr<T>& p, bool registerAsObserver = true);
    explicit RelinkableHandle(SharedPtr<T>&& p, bool registerAsObserver = true);

    void LinkTo(const SharedPtr<T>& h, bool registerAsObserver = true);
    void LinkTo(SharedPtr<T>&& h, bool registerAsObserver = true);
    void reset();
};

template <class T>
inline RelinkableHandle<T>::RelinkableHandle(const SharedPtr<T>& p, bool registerAsObserver)
    : Handle<T>(p, registerAsObserver) 
{}

template <class T>
inline RelinkableHandle<T>::RelinkableHandle(SharedPtr<T>&& p, bool registerAsObserver)
    : Handle<T>(std::move(p), registerAsObserver) 
{}

template <class T>
inline void RelinkableHandle<T>::LinkTo(const SharedPtr<T>& h, bool registerAsObserver)
{
    this->_link->LinkTo(h, registerAsObserver);
}

template <class T>
inline void RelinkableHandle<T>::LinkTo(SharedPtr<T>&& h, bool registerAsObserver)
{
    this->_link->LinkTo(std::move(h), registerAsObserver);
}

template <class T>
inline void RelinkableHandle<T>::reset() 
{
    this->_link->LinkTo(nullptr, true);
}