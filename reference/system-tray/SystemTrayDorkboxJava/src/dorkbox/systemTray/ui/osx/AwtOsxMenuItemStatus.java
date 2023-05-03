/*
 * Copyright 2022 dorkbox, llc
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
package dorkbox.systemTray.ui.osx;

import static java.awt.Font.DIALOG;

import java.awt.Font;
import java.awt.MenuItem;

import dorkbox.systemTray.Status;
import dorkbox.systemTray.peer.StatusPeer;
import dorkbox.util.SwingUtil;

class AwtOsxMenuItemStatus implements StatusPeer {

    private final AwtOsxMenu parent;
    private final MenuItem _native = new MenuItem();

    AwtOsxMenuItemStatus(final AwtOsxMenu parent) {
        this.parent = parent;

        // status is ALWAYS at 0 index...
        parent._native.insert(_native, 0);
    }

    @Override
    public
    void setText(final Status menuItem) {
        SwingUtil.invokeLater(()->{
            Font font = _native.getFont();
            if (font == null) {
                font = new Font(DIALOG, Font.BOLD, 12); // the default font used for dialogs.
            }
            else {
                font = font.deriveFont(Font.BOLD);
            }

            _native.setFont(font);
            _native.setLabel(menuItem.getText());

            // this makes sure it can't be selected
            _native.setEnabled(false);
        });
    }

    @Override
    public
    void remove() {
        SwingUtil.invokeLater(()->parent._native.remove(_native));
    }
}
