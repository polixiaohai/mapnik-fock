#ifndef MAPNIK_SAVE_MAP_SERIALIZE_HPP
#define MAPNIK_SAVE_MAP_SERIALIZA_HPP

#include "parse_path.hpp"
#include "ptree_helpers.hpp"
#include "symbolizer.hpp"
#include "symbolizer_utils.hpp"
#include "transform/transform_processor.hpp"
#include <mapnik/util/conversions.hpp>
#include <typeinfo>
#include <boost/property_tree/ptree.hpp>
#include <mapnik/enumeration.hpp>
#include <mapnik/symbolizer_base.hpp>
#include <mapnik/save_map.hpp>

namespace mapnik {
template<typename Meta>
class MAPNIK_DECL serialize_symbolizer_property
{
  public:
    serialize_symbolizer_property(Meta const& meta, boost::property_tree::ptree& node, bool explicit_defaults, int type)
        : meta_(meta)
        , node_(node)
        , explicit_defaults_(explicit_defaults)
        , type_(type)
    {}

    void operator()(mapnik::enumeration_wrapper const& e) const
    {
        auto const& convert_fun_ptr(std::get<1>(meta_));
        if (convert_fun_ptr)
        {
            node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), convert_fun_ptr(e));
        }
    }

    void operator()(path_expression_ptr const& expr) const
    {
        if (expr)
        {
            node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), path_processor::to_string(*expr));
        }
    }

    void operator()(text_placements_ptr const& expr) const
    {
        if (expr)
        {
            mapnik::serialize_text_placements(node_, expr, explicit_defaults_, type_);
        }
    }

    void operator()(raster_colorizer_ptr const& expr) const
    {
        if (expr)
        {
            mapnik::serialize_raster_colorizer(node_, expr, explicit_defaults_);
        }
    }

    void operator()(transform_type const& expr) const
    {
        if (expr)
        {
            node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), transform_processor_type::to_string(*expr));
        }
    }

    void operator()(expression_ptr const& expr) const
    {
        if (expr)
        {
            node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), mapnik::to_expression_string(*expr));
        }
    }

    void operator()(dash_array const& dash) const
    {
        std::ostringstream os;
        for (std::size_t i = 0; i < dash.size(); ++i)
        {
            os << dash[i].first << ", " << dash[i].second;
            if (i + 1 < dash.size())
                os << ",";
        }
        node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), os.str());
    }

    void operator()(group_symbolizer_properties_ptr const& properties) const
    {
        if (properties)
        {
            mapnik::serialize_group_symbolizer_properties(node_, properties, explicit_defaults_);
        }
    }

    template<typename T>
    void operator()(T const& val) const
    {
        node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), val);
    }

    // 特化为double的版本
    void operator()(double const& val) const
    {
        std::string val_str;
        if (mapnik::util::to_string(val_str, val))
        {
            node_.put("<xmlattr>." + std::string(std::get<0>(meta_)), std::move(val_str));
        }
    }

  private:
    Meta const& meta_;
    boost::property_tree::ptree& node_;
    bool explicit_defaults_;
    int type_; // 1: text_text_symbolizer, 2: shield_symbolizer
};

class MAPNIK_DECL serialize_symbolizer
{
  public:
    serialize_symbolizer(ptree& r, bool explicit_defaults)
        : rule_(r)
        , explicit_defaults_(explicit_defaults)
    {}

    template<typename Symbolizer>
    void operator()(Symbolizer const& sym)
    {
        ptree& sym_node = rule_.push_back(ptree::value_type(symbolizer_traits<Symbolizer>::name(), ptree()))->second;
        serialize_symbolizer_properties(sym_node, sym);
    }

  private:

    void serialize_symbolizer_properties(ptree& sym_node, symbolizer_base const& sym)
    {
        int type_id = get_type(sym);
        for (auto const& prop : sym.properties)
        {
            util::apply_visitor(serialize_symbolizer_property<property_meta_type>(get_meta(prop.first),
                                                                                  sym_node,
                                                                                  explicit_defaults_,
                                                                                  type_id),
                                prop.second);
        }
    }

    int get_type(symbolizer_base const& sym) const
    {
        return typeid(sym) == typeid(mapnik::text_symbolizer)
                 ? 7
                 : (typeid(sym) == typeid(mapnik::shield_symbolizer) ? 6 : 0);
    }

    ptree& rule_;
    bool explicit_defaults_;
};

class MAPNIK_DECL serialize_group_layout
{
  public:
    serialize_group_layout(ptree& parent_node, bool explicit_defaults)
        : parent_node_(parent_node)
        , explicit_defaults_(explicit_defaults)
    {}

    void operator()(simple_row_layout const& layout) const
    {
        ptree& layout_node = parent_node_.push_back(ptree::value_type("SimpleLayout", ptree()))->second;

        simple_row_layout dfl;
        if (explicit_defaults_ || layout.get_item_margin() != dfl.get_item_margin())
        {
            set_attr(layout_node, "item-margin", layout.get_item_margin());
        }
    }

    void operator()(pair_layout const& layout) const
    {
        ptree& layout_node = parent_node_.push_back(ptree::value_type("PairLayout", ptree()))->second;

        pair_layout dfl;
        if (explicit_defaults_ || layout.get_item_margin() != dfl.get_item_margin())
        {
            set_attr(layout_node, "item-margin", layout.get_item_margin());
        }
        if (explicit_defaults_ || layout.get_max_difference() != dfl.get_max_difference())
        {
            set_attr(layout_node, "max-difference", layout.get_max_difference());
        }
    }

    template<typename T>
    void operator()(T const&) const
    {}

  private:
    ptree& parent_node_;
    bool explicit_defaults_;
};

} // namespace mapnik

#endif // MAPNIK_SAVE_MAP_SERIALIZA_HPP
