/*
  Copyright (c) 2013 Matthew Stump

  This file is part of libmutton.

  libmutton is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  libmutton is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __MUTTON_QUERY_PRINTER_HPP_INCLUDED__
#define __MUTTON_QUERY_PRINTER_HPP_INCLUDED__

#include "query_parser.hpp"

namespace mtn {

struct query_printer_t :
    boost::static_visitor<std::string>
{
    std::string
    operator()(const std::string& v) const
    {
        return v;
    }

    std::string
    operator()(const mtn::integer v) const
    {
        return boost::lexical_cast<std::string>(v);
    }

    std::string
    operator()(const mtn::op_and& o) const
    {
        return print("&", o.children.begin(), o.children.end());
    }

    std::string
    operator()(const mtn::op_or& o) const
    {
        return print("|", o.children.begin(), o.children.end());
    }

    std::string
    operator()(const mtn::op_xor& o) const
    {
        return print("^", o.children.begin(), o.children.end());
    }

    std::string
    operator()(const mtn::op_not& o) const
    {
        return std::string("(! ") + boost::apply_visitor(*this, o.child) + ")";
    }

    std::string
    operator()(const mtn::op_range& o) const
    {
        return std::string("(range ")
            + boost::lexical_cast<std::string>(o.start)
            + " "
            + boost::lexical_cast<std::string>(o.limit)
            + ")";
    }

    std::string
    operator()(const mtn::op_regex& o) const
    {
        return std::string("(regex \"") + o.pattern + "\")";
    }

    std::string
    operator()(const mtn::op_slice& o) const
    {
        if (o.values.empty()) {
            return std::string("(slice \"") + o.index + "\")";
        }
        else {
            return print("slice \"" + o.index + "\"", o.values.begin(), o.values.end());
        }
    }

    template<class Iterator>
    inline std::string
    print(const std::string& op,
          Iterator it,
          Iterator end) const
    {
        std::list<std::string> temp;
        for (; it != end; ++it) {
            temp.push_back(boost::apply_visitor(*this, *it));
        }

        return std::string("(") + op + " " + boost::algorithm::join(temp, " ") + ")";
    }
};

} // namespace mtn

#endif // __MUTTON_QUERY_PRINTER_HPP_INCLUDED__
