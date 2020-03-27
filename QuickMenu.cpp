#include "QuickMenu.h"

bool QuickMenu::init( std::string type, std::string name, Node *parent )
{
    if ( !is_valid_type( type ) || name == "" ) {
        return false;
    }
    _parent = parent;
    _type   = type;
    origin  = Director::getInstance()->getVisibleOrigin();
    vsize   = Director::getInstance()->getVisibleSize();
    setName( name );
    
    if ( !init_bg() ) {
        return false;
    }

    init_sounds();
    init_callbacks();
    add_menu_items();

    items_count = _selections.size();
    _current  = first_enabled();
    return true;
}

void QuickMenu::init_sounds( void )
{
    
}

bool QuickMenu::cursor( void )
{
    
}

bool QuickMenu::init_bg( void )
{
    
}

void QuickMenu::add_callback( std::string name, const std::function<void()> func )
{
    _callbacks[name] = func;
}

void QuickMenu::add_menu_items( void )
{
    
}

void QuickMenu::add_menu_item( std::string text, float size, float y_offset, 
                            bool enabled, const std::function<void()> func )
{
    
}

void QuickMenu::show( void )
{
    if ( getParent() == nullptr ) {
        _parent->addChild( this );
        cursor();
    }
}

void QuickMenu::hide( void )
{
    removeFromParent();
}

void QuickMenu::lock_state( std::string state )
{
    if ( state != "locked" && state != "unlocked" ) {
        _lock_state = "locked";
        return;
    }
    _lock_state = state;
}

void QuickMenu::next( void )
{
    if ( _lock_state != "locked" ) {
        if ( _current < items_count - 1 ) {
            _cursor->setPosition(
                Vec2(
                    _cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'n' ) )->getPosition().y
                )
            );
            AudioEngine::play2d( _sounds["cursor-move"] );
        }
    }
}

void QuickMenu::prev( void )
{
    if ( _lock_state != "locked" ) {
        if ( _current > 0 ) {
            _cursor->setPosition(
                Vec2(
                    _cursor->getPosition().x,
                    _selections.at( next_enabled( &_current, 'p' ) )->getPosition().y
                )
            );
            AudioEngine::play2d( _sounds["cursor-move"] );
        }
    }
}

short QuickMenu::next_enabled( short *index, char ch ) const
{
    short accum = (ch == 'n') ? 1 : -1;
    short temp = *index;

    for ( auto item : _selections ) {
        temp += accum ;
        if ( temp == 0 || temp == items_count - 1 ) {
            return (_selections.at( temp )->GetState() == "enabled") ? (*index = temp) : *index ;
        }
        if ( _selections.at( temp )->GetState() == "enabled" ) {
            return (*index = temp);
        }
    }
}

void QuickMenu::select( void )
{
    if ( _lock_state != "locked" ) {
        AudioEngine::play2d( _sounds["cursor-select"] );
        auto selection = GetSelection();
        auto callback  = selection->GetCallback();
        if ( callback ) {
            callback->execute();
        }
    }
}

short QuickMenu::first_enabled( void ) const
{
    short i = 0;
    for ( auto selection : _selections ) {
        if ( selection->GetState() == "enabled" ) {
            return i;
        }
        i++;
    }
    return i;
}

d_lan::MenuItem * QuickMenu::GetSelection( void ) const {
    return _selections.at( _current );
}

bool QuickMenu::is_valid_type( std::string type ) const
{
    if ( type != "menu" && type != "submenu" )
        return false;
    return true;
}