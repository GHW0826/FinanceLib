#pragma once

/* 
	���� ������ �׸� ���� ���� �ڵ�
	Ŭ������ ��� �ν��Ͻ� �纻�� ������ ���� ����
	�翬�� ������ ����Ʈ �����͸� ���� ������ �� �ִ�. 
	�׷� ��� �����Ͱ� �ٸ� �������� �ٽ� ����Ǹ� ����ȴ�.
	T�� Observable���� ��ӵǾ�� ��
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

// ��������(Observable) ��ü�� ���� �翬�� ������ �ڵ�
/*
    �� Ŭ������ �ν��Ͻ��� �ٸ� �������� ��ü�� ����Ű���� �翬��(relink)�� �� �ִ�.
    �̷��� �翬��Ǹ�, �ش� �ν��Ͻ��� �����Ͽ� ������ ��� �ڵ鿡�� ������ ���ĵ�

    pre Ŭ���� T�� Observable�� ��ӹ޾ƾ� ��.

    \warning `Handle` ������ �����Ͽ� <tt>registerAsObserver</tt>�� ���õ� �̽��� ����..
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