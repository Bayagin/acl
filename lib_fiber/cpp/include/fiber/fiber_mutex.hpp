#pragma once
#include "fiber_cpp_define.hpp"

struct ACL_FIBER_MUTEX;

namespace acl {

/**
 * ������ͬһ�߳��ڵ�Э��֮���Լ���ͬ�߳�֮���Э��֮��Ļ�����, ͬʱ����������
 * �߳�֮���Լ�Э��������߳�֮��Ļ���.
 */
class FIBER_CPP_API fiber_mutex
{
public:
	fiber_mutex(void);
	~fiber_mutex(void);

	/**
	 * �ȴ�������
	 * @return {bool} ���� true ��ʾ�����ɹ��������ʾ�ڲ�����
	 */
	bool lock(void);

	/**
	 * ���Եȴ�������
	 * @return {bool} ���� true ��ʾ�����ɹ��������ʾ�����ڱ�ռ��
	 */
	bool trylock(void);

	/**
	 * ������ӵ�����ͷ�����֪ͨ�ȴ���
	 * @return {bool} ���� true ��ʾ֪ͨ�ɹ��������ʾ�ڲ�����
	 */
	bool unlock(void);

public:
	/**
	 * ���� C �汾�Ļ���������
	 * @return {ACL_FIBER_MUTEX*}
	 */
	ACL_FIBER_MUTEX* get_mutex(void) const
	{
		return mutex_;
	}

private:
	ACL_FIBER_MUTEX* mutex_;

	fiber_mutex(const fiber_mutex&);
	void operator=(const fiber_mutex&);
};

} // namespace acl
