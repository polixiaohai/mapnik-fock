/*****************************************************************************
 *
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2024 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef MAPNIK_SAVE_MAP_HPP
#define MAPNIK_SAVE_MAP_HPP

// mapnik
#include <mapnik/rule.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/layer.hpp>
#include <mapnik/feature_type_style.hpp>
#include <mapnik/debug.hpp>
#include <mapnik/symbolizer.hpp>
#include <mapnik/ptree_helpers.hpp>
#include <mapnik/expression_string.hpp>
#include <mapnik/raster_colorizer.hpp>
#include <mapnik/text/placements/simple.hpp>
#include <mapnik/text/placements/list.hpp>
#include <mapnik/text/placements/dummy.hpp>
#include <mapnik/image_compositing.hpp>
#include <mapnik/image_scaling.hpp>
#include <mapnik/image_filter.hpp>
#include <mapnik/image_filter_types.hpp>
#include <mapnik/parse_path.hpp>
#include <mapnik/symbolizer_utils.hpp>
#include <mapnik/transform/transform_processor.hpp>
#include <mapnik/group/group_rule.hpp>
#include <mapnik/group/group_layout.hpp>
#include <mapnik/group/group_symbolizer_properties.hpp>
#include <mapnik/util/variant.hpp>
#include <mapnik/util/variant_io.hpp>
#pragma GCC diagnostic push
#include <mapnik/warning_ignore.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/version.hpp>
#pragma GCC diagnostic pop

// stl
#include <string>

namespace mapnik {
class Map;

using boost::property_tree::ptree;

MAPNIK_DECL void serialize_text_placements(ptree& node, text_placements_ptr const& p, bool explicit_defaults, int type);

MAPNIK_DECL void
  serialize_raster_colorizer(ptree& sym_node, raster_colorizer_ptr const& colorizer, bool explicit_defaults);

MAPNIK_DECL void serialize_group_symbolizer_properties(ptree& sym_node,
                                                       group_symbolizer_properties_ptr const& properties,
                                                       bool explicit_defaults);

MAPNIK_DECL void serialize_group_rule(ptree& parent_node, const group_rule& r, bool explicit_defaults);

MAPNIK_DECL void serialize_rule(ptree& style_node, rule const& r, bool explicit_defaults);

MAPNIK_DECL void
  serialize_style(ptree& map_node, std::string const& name, feature_type_style const& style, bool explicit_defaults);

MAPNIK_DECL void serialize_fontset(ptree& map_node, std::string const& name, font_set const& fontset);

MAPNIK_DECL void serialize_datasource(ptree& layer_node, datasource_ptr datasource);

MAPNIK_DECL void serialize_parameters(ptree& map_node, mapnik::parameters const& params);

MAPNIK_DECL void serialize_layer_extra_parameters(ptree& layer_node, mapnik::parameters const& params);

MAPNIK_DECL void serialize_layer(ptree& map_node, layer const& lyr, bool explicit_defaults);

MAPNIK_DECL void serialize_map(ptree& pt, Map const& map, bool explicit_defaults);

MAPNIK_DECL void save_map(Map const& map, std::string const& filename, bool explicit_defaults = false);

MAPNIK_DECL std::string save_map_to_string(Map const& map, bool explicit_defaults = false);
} // namespace mapnik

#endif // MAPNIK_SAVE_MAP_HPP
