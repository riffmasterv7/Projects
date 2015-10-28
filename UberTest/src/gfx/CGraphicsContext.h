
#ifndef CGRAPHICSCONTEXT
#define CGRAPHICSCONTEXT

struct SGraphicsContextPrivate;

class CGraphicsContext
{
public:

	CGraphicsContext();
	~CGraphicsContext();

	static bool createDisplay(void * nativeDisplayId, void * nativeSurface, CGraphicsContext& context);
	static bool createContext(CGraphicsContext& context, const CGraphicsContext * sharedContext);

	static void activate(CGraphicsContext& context);

	static void present(CGraphicsContext& context);

	static void destroyContext(CGraphicsContext& context);
	static void destroyDisplay(CGraphicsContext& context);

private:

	SGraphicsContextPrivate& mPrivate;

};

#endif