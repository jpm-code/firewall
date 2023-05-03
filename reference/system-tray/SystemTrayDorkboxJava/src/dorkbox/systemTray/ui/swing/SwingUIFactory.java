/*
 * Copyright 2021 dorkbox, llc
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package dorkbox.systemTray.ui.swing;

import java.awt.Color;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JSeparator;
import javax.swing.plaf.MenuItemUI;
import javax.swing.plaf.PopupMenuUI;
import javax.swing.plaf.SeparatorUI;

import dorkbox.systemTray.Entry;
import dorkbox.systemTray.Menu;

/**
 * Factory to allow for Look & Feel of the Swing UI components in the SystemTray. Provide an implementation to customize the Swing L&F.
 *
 * NOTICE: components can ALSO have different sizes attached to them, resulting in different sized components
 * mini
 *       myButton.putClientProperty("JComponent.sizeVariant", "mini");
 * small
 *       mySlider.putClientProperty("JComponent.sizeVariant", "small");
 * large
 *       myTextField.putClientProperty("JComponent.sizeVariant", "large");
 */
public
interface SwingUIFactory {

    /**
     * Allows one to specify the Look & Feel of the menus (The main SystemTray and sub-menus)
     *
     * @param jPopupMenu the swing JPopupMenu that is displayed when one clicks on the System Tray icon
     * @param entry the entry which is bound to the menu, or NULL if it is the main SystemTray menu.
     *
     * @return the UI used to customize the Look & Feel of the SystemTray menu + sub-menus
     */
    PopupMenuUI getMenuUI(JPopupMenu jPopupMenu, Menu entry);

    /**
     * Allows one to specify the Look & Feel of a menu entry
     *
     * @param jMenuItem the swing JMenuItem that is displayed in the menu
     * @param entry the entry which is bound to the JMenuItem. Can be NULL during initialization.
     *
     * @return the UI used to customize the Look & Feel of the menu entry
     */
    MenuItemUI getItemUI(JMenuItem jMenuItem, Entry entry);

    /**
     * Allows one to specify the Look & Feel of a menu separator entry
     *
     * @param jSeparator the swing JSeparator that is displayed in the menu
     *
     * @return the UI used to customize the Look & Feel of a menu separator entry
     */
    SeparatorUI getSeparatorUI(JSeparator jSeparator);

    /**
     * This saves a vector CheckMark to a correctly sized PNG file. The checkmark image will ALWAYS be centered in the targetImageSize
     * (which is square)
     *
     * @param color the color of the CheckMark
     * @param checkMarkSize the size of the CheckMark inside the image. (does not include padding)
     *
     * @return the full path to the checkmark image
     */
    String getCheckMarkIcon(final Color color, final int checkMarkSize, final int targetImageSize);
}
