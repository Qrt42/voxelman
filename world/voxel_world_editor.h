/*
Copyright (c) 2020 Péter Magyar

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

#ifndef VOXEL_WORLD_EDITOR_PLUGIN_H
#define VOXEL_WORLD_EDITOR_PLUGIN_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"

class VoxelWorld;
class SpatialEditorPlugin;

class VoxelWorldEditor : public VBoxContainer {
	GDCLASS(VoxelWorldEditor, VBoxContainer);

public:
	bool forward_spatial_input_event(Camera *p_camera, const Ref<InputEvent> &p_event);

	void edit(VoxelWorld *p_world);

	VoxelWorldEditor();
	VoxelWorldEditor(EditorNode *p_editor);
	~VoxelWorldEditor();

	HBoxContainer *spatial_editor_hb;

protected:
	static void _bind_methods();
	void _node_removed(Node *p_node);

private:
	VoxelWorld *_world;
};

class VoxelWorldEditorPlugin : public EditorPlugin {

	GDCLASS(VoxelWorldEditorPlugin, EditorPlugin);

	VoxelWorldEditor *voxel_world_editor;
	EditorNode *editor;

protected:
	void _notification(int p_what);

public:
	virtual bool forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) { return voxel_world_editor->forward_spatial_input_event(p_camera, p_event); }
	virtual String get_name() const { return "VoxelWorld"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	VoxelWorldEditorPlugin(EditorNode *p_node);
	~VoxelWorldEditorPlugin();
};

#endif