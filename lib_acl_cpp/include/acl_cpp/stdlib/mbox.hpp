#pragma once
#include "../acl_cpp_define.hpp"
#include <assert.h>
#include "box.hpp"

namespace acl {

// internal functions being used
void*  mbox_create(bool mpsc);
void   mbox_free(void*, void (*free_fn)(void*));
bool   mbox_send(void*, void*);
void*  mbox_read(void*, int, bool*);
size_t mbox_nsend(void*);
size_t mbox_nread(void*);

/**
 * ���������߳�֮�䡢Э��֮�����ͨ�ŵ��࣬�ڲ�ʵ�ֲ����������� + IO ͨ����
 * ��Ϸ�ʽʵ��
 *
 * ʾ��:
 *
 * class myobj {
 * public:
 *     myobj(void) {}
 *     ~myobj(void) {}
 *     
 *     void run(void)
 *     {
 *         printf("hello world!\r\n");
 *     }
 * };
 * 
 * acl::mbox<myobj> mbox;
 *
 * void thread_producer(void)
 * {
 *     myobj* o = new myobj;
 *     mbox.push(o);
 * }
 * 
 * void thread_consumer(void)
 * {
 *     myobj* o = mbox.pop();
 *     o->run();
 *     delete o;
 * }
 */

template<typename T>
class mbox : public box<T> {
public:
	/**
	 * ���췽��
	 * @param free_obj {bool} �� tbox ����ʱ���Ƿ��Զ���鲢�ͷ�
	 *  δ�����ѵĶ�̬����
	 * @param mpsc {bool} �Ƿ�Ϊ��������-��������ģʽ
	 */
	mbox(bool free_obj = true, bool mpsc = true)
	: free_obj_(free_obj)
	{
		mbox_ = mbox_create(mpsc);
		assert(mbox_);
	}

	~mbox(void)
	{
		mbox_free(mbox_, free_obj_ ? mbox_free_fn : NULL);
	}

	/**
	 * ������Ϣ����
	 * @param t {T*} �ǿ���Ϣ����
	 * @param dummy {bool} Ŀǰ���κ��ô�������Ϊ���� tbox �ӿ�һ��
	 * @return {bool} �����Ƿ�ɹ�
	 * @override
	 */
	bool push(T* t, bool dummy = false)
	{
		(void) dummy;
		return mbox_send(mbox_, t);
	}

	/**
	 * ������Ϣ����
	 * @param timeout {int} >= 0 ʱ���ö��ȴ���ʱʱ��(���뼶��)������
	 *  ��Զ�ȴ�ֱ��������Ϣ��������
	 * @param found {bool*} �ǿ�ʱ��������Ƿ�����һ����Ϣ������Ҫ����
	 *  �������ݿն���ʱ�ļ��
	 * @return {T*} �� NULL ��ʾ���һ����Ϣ���󣬷��� NULL ʱ����Ҫ����һ
	 *  ����飬��������� push ��һ���ն���NULL������������Ҳ���� NULL��
	 *  ����ʱ��Ȼ��Ϊ�����һ����Ϣ����ֻ����Ϊ�ն������ wait_ms ����
	 *  Ϊ -1 ʱ���� NULL ��Ȼ��Ϊ�����һ������Ϣ������� wait_ms ����
	 *  ���� 0 ʱ���� NULL����Ӧ�ü�� found ������ֵΪ true ���� false ��
	 *  �ж��Ƿ�����һ������Ϣ����
	 * @override
	 */
	T* pop(int timeout = -1, bool* found = NULL)
	{
		return (T*) mbox_read(mbox_, timeout, found);
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����͵���Ϣ��
	 * @return {size_t}
	 */
	size_t push_count(void) const
	{
		return mbox_nsend(mbox_);
	}

	/**
	 * ͳ�Ƶ�ǰ�Ѿ����յ�����Ϣ��
	 * @return {size_t}
	 */
	size_t pop_count(void) const
	{
		return mbox_nread(mbox_);
	}

private:
	void* mbox_;
	bool  free_obj_;

	static void mbox_free_fn(void* o)
	{
		T* t = (T*) o;
		delete t;
	}
};

} // namespace acl
