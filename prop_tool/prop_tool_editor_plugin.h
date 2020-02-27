/*
Copyright (c) 2019-2020 Péter Magyar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef PROP_TOOL_EDITOR_PLUGIN_H
#define PROP_TOOL_EDITOR_PLUGIN_H

#include "editor/editor_plugin.h"
#include "editor/pane_drag.h"

#include "scene/gui/graph_edit.h"
#include "scene/gui/menu_button.h"
#include "scene/main/viewport.h"

#include "../props/prop_data.h"

#include "scene/gui/viewport_container.h"

#include "editor/plugins/spatial_editor_plugin.h"

class PropDataEditor : public HSplitContainer {
	GDCLASS(PropDataEditor, HSplitContainer);

public:
	void edit(const Ref<PropData> &prop);

	PropDataEditor() {}
	PropDataEditor(EditorNode *p_editor);
	~PropDataEditor();

protected:
	static void _bind_methods();

private:
};

class PropDataEditorPlugin : public EditorPlugin {

	GDCLASS(PropDataEditorPlugin, EditorPlugin);

public:
	virtual String get_name() const { return "PropData"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	PropDataEditorPlugin(EditorNode *p_node);
	~PropDataEditorPlugin();

protected:
private:
	PropDataEditor *prop_editor;
	ToolButton *prop_editor_button;
	EditorNode *editor;
};

#endif