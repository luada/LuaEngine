#pragma once

#define FORCE_LINK_DECL(compoment)	bool force_link_##compoment##_ = false;

#define FORCE_LINK_IMPL(compoment)					\
	struct SForceLinker_##compoment					\
	{												\
		SForceLinker_##compoment()					\
		{											\
			extern bool force_link_##compoment##_;	\
			force_link_##compoment##_ = true;		\
		}											\
		~SForceLinker_##compoment(){}				\
	} stForceLink_##compoment##_;



#define ScriptWrapImplBegin(className)						\
	public:													\
		virtual void* GetThis(const char* pszClassName = NULL)\
		{													\
			if(pszClassName == NULL)						\
			{												\
				return this;								\
			}												\
			return _GetThis(pszClassName);					\
		}													\
		virtual const char* ClassName() const				\
		{													\
			return _ClassName();							\
		}													\
		const char* _ClassName() const						\
		{													\
			static const char* s_pszClassName = #className; \
			return s_pszClassName;							\
		}													\
		void* _GetThis(const char* pszClassName)			\
		{													\
			if(strcmp(pszClassName, _ClassName()) == 0)		\
			{												\
				return this;								\
			}												\
			void* This = NULL;								\


#define ScriptWrapImplEnd()		return This;}				\


#define ScriptWrapItem(Base)								\
		This = Base::_GetThis(pszClassName);				\
		if(This != NULL)									\
		{													\
			return This;									\
		}													\



