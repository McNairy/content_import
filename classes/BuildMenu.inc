<?php

/**
 * Reference: http://befused.com/drupal/programmatically-save-menu-links
 */
class BuildMenu {
    private $_menus    = [];
    private $_menuList = [];
    private $_menuTree = [];

    public function __construct() {
        $this->_menus = menu_get_names();
    }

    public function getMenus() {
        return $this->buildMenus();
    }

    private function buildMenu($name) {
        $menu = menu_build_tree($name);
        $data = [];

        foreach($menu as $k => $v) {
            $name                 = $v['link']['menu_name'];
            $mlid                 = $v['link']['mlid'];
            $title                = $v['link']['link_title'];

            $data['menu_name']    = $name;
            $data['mlid']         = $mlid;;
            $data['link_title']   = $title;
        }

        return $data;
    }

    private function buildMenus() {
        // foreach ($this->_menus as $key => $value) {
        //     echo "Key: $key, Value: $value<br/>";
        // }
        foreach($this->_menus as $menu => $menu_name) {
            array_push($this->_menuTree, $this->buildMenu($menu_name));
        }
        return $this->_menuTree;
    }

    /**
     * die and dump
     * @param  mixed $debug Anything to output to screen
     * @return string        debug info
     */
    protected static function dd($debug) {
        if(!empty($debug))
        {
            print_r($debug);
        }

        $debug = debug_backtrace();

        die("<p style='color: red;font-size: large'>" . __FILE__ . ":" . $debug[0]['line']."</p>");
    }

    public function render() {
        $menus = $this->getMenus();
        $html  = '<select>';

        foreach($menus as $menu) {
            $html .= <<<HTML
                <option value="{$menu['menu_name']}">{$menu['link_title']}</option>
HTML;
        }

        $html .= "</select>";

        return $html;
    }

    public function createStandardMenuItem($item) {
        $item = array (
            'link_title' => $item['link_title'],
            'link_path'  => 'example/path',
            'menu_name'  => $item['menu_name'],
            'weight'     => 0,
            'plid'       => $item['menu_name'],
          );

        menu_link_save($item);
    }

    public function createMenuForNode() {
        $path = drupal_lookup_path('source', 'example-node-alias');
        $item = array (
            'link_title' => $item['link_title'],
            'link_path' => $path,
            'router_path' => 'node/%',
            'menu_name' => $item['menu_name'],
            'weight' => 0,
            'plid' => $item['menu_name'],
        );

        //menu_link_save($item); //not ready, need to learn to set $path variable properly
    }
}