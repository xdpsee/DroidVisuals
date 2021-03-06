/* Libvisual - The audio visualisation framework.
 *
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id: lv_transform.h,v 1.6 2006/01/27 20:18:26 synap Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef _LV_TRANSFORM_H
#define _LV_TRANSFORM_H

#include <libvisual/lv_audio.h>
#include <libvisual/lv_video.h>
#include <libvisual/lv_plugin.h>

/**
 * @defgroup VisTransform VisTransform
 * @{
 */

VISUAL_BEGIN_DECLS

#define VISUAL_TRANSFORM(obj)				(VISUAL_CHECK_CAST ((obj), VisTransform))
#define VISUAL_TRANSFORM_PLUGIN(obj)		(VISUAL_CHECK_CAST ((obj), VisTransformPlugin))

/**
 * Type defination that should be used in plugins to set the plugin type for a transform plugin.
 */
#define VISUAL_PLUGIN_TYPE_TRANSFORM	"Libvisual:core:transform"

typedef struct _VisTransform VisTransform;
typedef struct _VisTransformPlugin VisTransformPlugin;

/* Transform plugin methodes */

/**
 * A transform plugin needs this signature to transform VisPalettes.
 *
 * @arg plugin Pointer to the VisPluginData instance structure.
 * @arg pal Pointer to the VisPalette that is to be morphed.
 *	Only 256 entry VisPalettes have to be supported.
 * @arg audio Optionally a pointer to the VisAudio, when requested.
 *
 * @return 0 on succes -1 on error.
 */
typedef int (*VisPluginTransformPaletteFunc)(VisPluginData *plugin, VisPalette *pal, VisAudio *audio);

/**
 * A transform plugin needs this signature to transform VisVideos.
 *
 * @arg plugin Pointer to the VisPluginData instance structure.
 * @arg video Pointer to the VisVideo that needs to be transformed.
 * @arg audio Optionally a pointer to the VisAudio, when requested.
 *
 * @return 0 on succes -1 on error.
 */
typedef int (*VisPluginTransformVideoFunc)(VisPluginData *plugin, VisVideo *video, VisAudio *audio);

/**
 * The VisTransform structure encapsulates the transform plugin and provides
 * abstract interfaces to the transform.
 *
 * Members in the structure shouldn't be accessed directly but instead
 * it's adviced to use the methods provided.
 *
 * @see visual_transform_new
 */
struct _VisTransform {
	VisObject	 object;		/**< The VisObject data. */

	VisPluginData	*plugin;		/**< Pointer to the plugin itself. */

	/* Video management and fake environments when needed */
	VisVideo	*video;			/**< Pointer to the target display video.
						 * @see visual_transform_set_video */
	VisPalette	*pal;			/**< Pointer to the VisPalette that is to be transformed.
						 * @see visual_transform_set_palette */
};

/**
 * The VisTransformPlugin structure is the main data structure
 * for the transform plugin.
 *
 * The transform plugin is used to transform videos and palettes
 * and can be used in visualisation pipelines.
 */
struct _VisTransformPlugin {
	VisObject			 object;	/**< The VisObject data. */
	VisPluginTransformPaletteFunc	 palette;	/**< Used to transform a VisPalette. Writes directly into the source. */
	VisPluginTransformVideoFunc	 video;		/**< Used to transform a VisVideo. Writes directly into the source. */

	int				 requests_audio;/**< When set on TRUE this will indicate that the Morph plugin
							  * requires an VisAudio context in order to render properly. */

	VisVideoAttributeOptions	 vidoptions;
};

/**
 * Gives the encapsulated VisPluginData from a VisTransform.
 *
 * @param transform Pointer of a VisTransform of which the VisPluginData needs to be returned.
 *
 * @return VisPluginData that is encapsulated in the VisTransform, possibly NULL.
 */
VisPluginData *visual_transform_get_plugin (VisTransform *transform);

/**
 * Gives a list of VisTransforms in the current plugin registry.
 *
 * @return An VisList containing the VisTransforms in the plugin registry.
 */
VisList *visual_transform_get_list (void);

/**
 * Gives the next transform plugin based on the name of a plugin.
 *
 * @see visual_transform_get_prev_by_name
 *
 * @param name The name of the current plugin, or NULL to get the first.
 *
 * @return The name of the next plugin within the list.
 */
const char *visual_transform_get_next_by_name (const char *name);

/**
 * Gives the previous transform plugin based on the name of a plugin.
 *
 * @see visual_transform_get_next_by_name
 *
 * @param name The name of the current plugin. or NULL to get the last.
 *
 * @return The name of the previous plugin within the list.
 */
const char *visual_transform_get_prev_by_name (const char *name);

/**
 * Checks if the transform plugin is in the registry, based on it's name.
 *
 * @param name The name of the plugin that needs to be checked.
 *
 * @return TRUE if found, else FALSE.
 */
int visual_transform_valid_by_name (const char *name);


/**
 * Creates a new transform from name, the plugin will be loaded but
 * won't be realized.
 *
 * @param transformname
 * 	The name of the plugin to load, or NULL to simply allocate a new
 * 	transform.
 *
 * @return A newly allocated VisTransform, optionally containing a loaded plugin. Or NULL on failure.
 */
VisTransform *visual_transform_new (const char *transformname);


/**
 * Initializes a VisTransform, this will set the allocated flag for
 * the object to FALSE. Should not be used to reset a VisTransform, or
 * on a VisTransform created by visual_transform_new().
 *
 * @see visual_transform_new
 *
 * @param transform Pointer to the VisTransform that is initialized.
 * @param transformname
 *	The name of the plugin to load, or NULL to simply initialize a new transform.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL or -VISUAL_ERROR_PLUGIN_NO_LIST on failure.
 */
int visual_transform_init (VisTransform *transform, const char *transformname);

/**
 * Realize the VisTransform. This also calls the plugin init function.
 *
 * @param transform Pointer to a VisTransform that needs to be realized.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL, -VISUAL_ERROR_PLUGIN_NULL or
 *	error values returned by visual_plugin_realize () on failure.
 *
 */
int visual_transform_realize (VisTransform *transform);

/**
 * This function negotiates the VisTransform with it's target video
 * that is set by visual_transform_set_video.  When needed it also
 * sets up size fitting environment and depth transformation
 * environment.
 *
 * @param transform Pointer to a VisTransform that needs negotiation.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL, -VISUAL_ERROR_PLUGIN_NULL, -VISUAL_ERROR_PLUGIN_REF_NULL
 * 	or -VISUAL_ERROR_TRANSFORM_NEGOTIATE on failure.
 */
int visual_transform_video_negotiate (VisTransform *transform);

/**
 * Gives the by the plugin natively supported depths
 *
 * @param transform Pointer to a VisTransform of which the supported depth of it's
 * 	  encapsulated plugin is requested.
 *
 * @return an OR value of the VISUAL_VIDEO_DEPTH_* values which can be checked against using AND on success,
 * 	-VISUAL_ERROR_TRANSFORM_NULL, -VISUAL_ERROR_PLUGIN_NULL or -VISUAL_ERROR_TRANSFORM_PLUGIN_NULL on failure.
 */
int visual_transform_get_supported_depth (VisTransform *transform);

VisVideoAttributeOptions *visual_transform_get_video_attribute_options (VisTransform *transform);

/**
 * Used to connect the target display it's VisVideo structure to the
 * VisTransform.
 *
 * Using the visual_video methods the screenbuffer, it's depth and
 * dimension and optionally it's pitch can be set so the transform
 * plugins know about their graphical environment and have a place to
 * draw.
 *
 * After this function it's most likely that
 * visual_transform_video_negotiate needs to be called.
 *
 * @see visual_video_new
 * @see visual_transform_video_negotiate
 *
 * @param transform Pointer to a VisTransform to which the VisVideo needs to be set.
 * @param video Pointer to a VisVideo which contains information about the target display and the pointer
 * 	  to it's screenbuffer.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL on failure.
 */
int visual_transform_set_video (VisTransform *transform, VisVideo *video);

/**
 * Used to override the palette that is extracted from the VisVideo
 * that is given using visual_transform_set_video. Always call this
 * function after visual_transform_set_video is called.
 *
 * @see visual_transform_set_video
 *
 * @param transform Pointer to a VisTransform to which the VisVideo needs to be set.
 * @param palette Pointer to the VisPalette which is used to override the palette in the VisTransform.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL on failure.
 */
int visual_transform_set_palette (VisTransform *transform, VisPalette *palette);

/**
 * This is called to run a VisTransform.
 *
 * @see visual_transform_run_video
 * @see visual_transform_run_palette
 *
 * @param transform Pointer to a VisTransform that needs to be runned.
 * @param audio Pointer to a VisAudio that contains all the audio data.
 *
 * return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL or error values returned by
 *	either visual_transform_run_video or visual_transform_run_palette on failure.
 */
int visual_transform_run (VisTransform *transform, VisAudio *audio);


/**
 * This is called to run the video part of a VisTransform.
 *
 * @see visual_transform_run
 *
 * @param transform Pointer to a VisTransform that needs to be runned.
 * @param audio Pointer to a VisAudio that contains all the audio data.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL, -VISUAL_ERROR_TRANSFORM_VIDEO_NULL
 *	or -VISUAL_ERROR_TRANSFORM_PLUGIN_NULL on failure.
 */
int visual_transform_run_video (VisTransform *transform, VisAudio *audio);

/**
 * This is called to run the palette part of a VisTransform.
 *
 * @see visual_transform_run
 *
 * @param transform Pointer to a VisTransform that needs to be runned.
 * @param audio Pointer to a VisAudio that contains all the audio data.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_TRANSFORM_NULL, -VISUAL_ERROR_TRANSFORM_PALETTE_NULL
 *	or -VISUAL_ERROR_TRANSFORM_PLUGIN_NULL on failure.
 */
int visual_transform_run_palette (VisTransform *transform, VisAudio *audio);

/**
 * @}
 */

VISUAL_END_DECLS

#endif /* _LV_TRANSFORM_H */
