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

#include <SFML/UI/TextField.hpp>
#include <SFML/UI/SFMLUtils.hpp>
#include <SFML/UI/Model/DefaultTextFieldModel.hpp>
#include <SFML/UI/ComponentEvent.hpp>

using namespace sf::ui;

TextField::TextField()
: Focusable(),
  m_text(), m_cursor(0), m_cursorShape(),
  m_maxLength(-1), m_canBeEmpty(true)
{
	m_model = new DefaultTextFieldModel;
	m_cursorShape.setFillColor(sf::Color::Black);
}

TextField::TextField(const sf::Texture& texture, const sf::Texture& textureFocused, sf::String const &text)
: Focusable(texture, textureFocused),
  m_text(), m_cursor(text.getSize()), m_cursorShape(),
  m_maxLength(-1), m_canBeEmpty(true)
{
	setText(text);
	m_model = new DefaultTextFieldModel;
	m_cursorShape.setFillColor(sf::Color::Black);
}

TextField::TextField(const sf::Texture& texture, const sf::Texture& textureFocused, const sf::Font& font, sf::String text)
: Focusable(texture, textureFocused),
  m_text(text, font), m_cursor(text.getSize()), m_cursorShape(),
  m_maxLength(-1), m_canBeEmpty(true)
{
	m_model = new DefaultTextFieldModel;
	m_cursorShape.setFillColor(sf::Color::Black);
}

TextField::~TextField()
{
	delete m_model;
}

void TextField::updateFixed(sf::Time delta)
{

}

void TextField::updateEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan) 
	{
	    int x = (event.type == sf::Event::MouseButtonPressed ? event.mouseButton.x : event.touch.x);
        int y = (event.type == sf::Event::MouseButtonPressed ? event.mouseButton.y : event.touch.y);
        
        if (event.type != sf::Event::MouseButtonPressed || event.mouseButton.button == sf::Mouse::Left)
		{
		    if (checkClickOn(event.mouseButton.x, event.mouseButton.y))
			    setFocused(true);
		    else if (m_text.getString().getSize() != 0 || m_canBeEmpty)
			    setFocused(false);
		}

		m_sprite.setTexture(*(m_focused ? m_textureFocused : m_texture), false);
#ifdef SFML_SYSTEM_ANDROID
		sf::Keyboard::setVirtualKeyboardVisible(m_focused);
#endif
	}
	else if (event.type == sf::Event::KeyPressed && m_focused) 
	{
		switch(event.key.code)
		{
		case sf::Keyboard::Return:
			if (m_text.getString().getSize() == 0 && !m_canBeEmpty)
				break;

			setFocused(false);
			m_cursor = m_text.getString().getSize();
			m_sprite.setTexture(*m_texture, false);
#ifdef SFML_SYSTEM_ANDROID
	    	sf::Keyboard::setVirtualKeyboardVisible(false);
#endif
			break;
		case sf::Keyboard::Left:
			if (m_cursor > 0)
				m_cursor--;

			break;
		case sf::Keyboard::Right:
			if (m_cursor < m_text.getString().getSize())
				m_cursor++;

			break;
		case sf::Keyboard::BackSpace:

			if (m_cursor != 0 && m_text.getString().getSize() > 0)
			{
			    m_cursor--;
			    
			    if (!deleteText(m_cursor))
			    {
			        m_cursor++;
			    }
			}

			break;
		case sf::Keyboard::Delete:

			if (m_cursor < m_text.getString().getSize())
			{
			    deleteText(m_cursor);
			}

			break;
		default:
			break;
		}
		
		updateCoord();
	}
	else if (event.type ==  sf::Event::TextEntered && (m_maxLength == -1 || m_text.getString().getSize() < static_cast<unsigned int>(m_maxLength)))
	{
		sf::Uint32 text = event.text.unicode;
		
		if (m_model->isCharAllowed(text) && m_focused)
		{
		    if (insertText(text, m_cursor))
		    {
		        updateCoord();
		    }
		}
	}
}

bool TextField::insertText(sf::Uint32 text, unsigned int index)
{
	sf::String string = m_text.getString();
	string.insert(index, text);
	m_cursor++;
	setText(string);

	sf::ui::ComponentEvent cevent;
	cevent.source = this;
	cevent.type = sf::ui::ComponentEvent::TextEntered;
	cevent.text.source = this;
	cevent.text.text = text;
	cevent.text.position = index;
	triggerEvent(cevent);
	
	return true;
}

bool TextField::deleteText(unsigned int index)
{
    sf::String str = m_text.getString();
    
    sf::ui::ComponentEvent cevent;
    cevent.source = this;
    cevent.type = sf::ui::ComponentEvent::TextDeleted;
    cevent.text.source = this;
    cevent.text.text = str[index];
    cevent.text.position = index;

    str.erase(index);
    setText(str);

    triggerEvent(cevent);
    
    return true;
}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Focusable::draw(target, states);
	target.draw(m_text);
	if (m_focused)
		target.draw(m_cursorShape);
}

void TextField::updateCoord()
{
	sf::Vector2f pos(m_sprite.getPosition()), size(SFMLUtils::getLocalSize(m_text));

	m_text.setOrigin(0, size.y/2.f);
	m_text.setPosition(pos.x + 5, pos.y + m_sprite.getGlobalBounds().height/2.f);

	m_cursorShape.setSize(sf::Vector2f(1, size.y));
	m_cursorShape.setOrigin(1, m_cursorShape.getSize().y/2.f);
	m_cursorShape.setPosition(m_text.getPosition());

	for (unsigned int i = 0; i < m_cursor; i++)
	{
		float x;
		x = static_cast<float>(m_text.getFont()->getGlyph(m_text.getString()[i], m_text.getCharacterSize(), false).advance);

		if (i != 0)
			x += static_cast<float>(m_text.getFont()->getKerning(m_text.getString()[i - 1], m_text.getString()[i], m_text.getCharacterSize()));
		else
			x += static_cast<float>(m_text.getFont()->getKerning(0, m_text.getString()[i], m_text.getCharacterSize()));

		m_cursorShape.move(x , 0);
	}
}

const TextFieldModel* TextField::getModel() const
{
	return m_model;
}

void TextField::setModel(TextFieldModel* model)
{
	if (model != m_model && model != 0)
	{
		delete m_model;
		m_model = model;
	}
}

const sf::String& TextField::getText() const
{
	return m_text.getString();
}

void TextField::setText(const sf::String& text)
{
	m_text.setString(text);
	
	if (m_cursor > m_text.getString().getSize())
	{
	    m_cursor = m_text.getString().getSize();
	    updateCoord();
	}
}

const sf::Font* TextField::getFont() const
{
	return m_text.getFont();
}

void TextField::setFont(const sf::Font& font)
{
	m_text.setFont(font);
}

unsigned int TextField::getFontSize() const
{
	return m_text.getCharacterSize();
}

void TextField::setFontSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}

sf::Color TextField::getFontColor() const
{
	return m_text.getColor();
}

void TextField::setFontColor(sf::Color color)
{
	m_text.setColor(color);
}

unsigned int TextField::getCursor() const
{
	return m_cursor;
}

void TextField::setCursor(unsigned int cursor)
{
	if (cursor <= m_text.getString().getSize())
		m_cursor = cursor;
}

int TextField::getMaxLength() const
{
	return m_maxLength;
}

void TextField::setMaxLength(int maxLength)
{
	m_maxLength = (maxLength > 0 ? maxLength : -1);
}

bool TextField::canBeEmpty() const
{
	return m_canBeEmpty;
}

void TextField::setCanBeEmpty(bool canBeEmpty)
{
	m_canBeEmpty = canBeEmpty;
}
