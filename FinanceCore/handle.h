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

};