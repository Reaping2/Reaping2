#ifndef INCLUDED_RENDER_RENDERER_H
#define INCLUDED_RENDER_RENDERER_H

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;
	Renderer();
	~Renderer();

	Projection mWorldProjector;
	Projection mUiProjector;
	Root mUiRoot;	//todo uimgr, vagy root singleton

	Registration mMouseMoveId;
	Registration mMousePressId;

	bool BeginRender();
	bool EndRender();
	void SetupRenderer(const Projection& Proj);
	// ez logikailag nem tuti, hogy teljesen korrekt, de a renderer az, aki ismeri a projectionoket
	// ki lehetne belole kulon osztalyba rakni esetleg
	// a screen mouse eventeket mindenesetre ezen a reszen lehet csak kezelni, a rendererben (hisz csak o tudja, h egyaltalan mi az a screen)
	void OnMouseMoveEvent(const ScreenMouseMoveEvent& Event);
	void OnMousePressEvent(const ScreenMousePressEvent& Event);
public:
	bool Render();
};

#endif//INCLUDED_RENDER_RENDERER_H
