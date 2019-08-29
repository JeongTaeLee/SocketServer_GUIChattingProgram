#pragma once

namespace JIGAPServerAPI
{
	class PoolObject
	{
	private:
	
		bool bActive = true;
	public:
		PoolObject() {}
		virtual ~PoolObject() {}

		virtual void OnActiveObject() {}
		virtual void OnUnActiveObject() {}
	
		void SetActive(bool bInIsActive);
		bool GetActive() { return bActive; }
	};
}

