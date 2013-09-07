/*
 *  Copyright © 2013 mathdu07
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SFMLUTILS_H_
#define SFMLUTILS_H_

#include <SFML/Graphics.hpp>

namespace sf
{
namespace ui
{

class SFMLUtils
{
	//CONSTRUCTORS/DESTRUCTORS -------------------------------------------
private:
	SFMLUtils();
public:
	virtual ~SFMLUtils();
	//--------------------------------------------------------------------

	//METHODS ------------------------------------------------------------
	static sf::Vector2f getLocalSize(const sf::Text& text);
	//--------------------------------------------------------------------

	//GETTERS/SETTERS ----------------------------------------------------

	//--------------------------------------------------------------------

	//FIELDS -------------------------------------------------------------

	//--------------------------------------------------------------------
};

} /* namespace ui */
} /* namespace sf */
#endif /* SFMLUTILS_H_ */
