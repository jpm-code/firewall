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

import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Image;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Window;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JDialog;
import javax.swing.JPopupMenu;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;

import dorkbox.systemTray.SystemTray;
import dorkbox.util.ScreenUtil;

/**
 * This custom popup is required if we want to be able to show images on the menu,
 *
 * This is our "golden standard" since we have 100% control over it on all platforms
 */
public
class TrayPopup extends JPopupMenu {
    private static final long serialVersionUID = 1L;

    // This lets us click OFF the menu to hide the menu. This will always show an entry in the taskbar
    // for java1.6 on linux (possibly others)
    private final JDialog hiddenDialog;

    private volatile File iconFile;

    @SuppressWarnings("unchecked")
    public
    TrayPopup(final String trayName) {
        super();
        setFocusable(true);

        // Initialize the hidden dialog as a headless, title-less dialog window
        hiddenDialog = new JDialog((Frame)null, trayName);
        hiddenDialog.setUndecorated(true);
        hiddenDialog.setDefaultCloseOperation(JDialog.DISPOSE_ON_CLOSE);
        hiddenDialog.setAlwaysOnTop(true);

        // on Linux, the following two entries will **MOST OF THE TIME** prevent the hidden dialog from showing in the task-bar
        hiddenDialog.getContentPane().setLayout(null);

        hiddenDialog.setType(Window.Type.POPUP);

        hiddenDialog.pack();
        hiddenDialog.setBounds(0,0,0,0);

        addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
                hiddenDialog.setVisible(false);
                hiddenDialog.toBack();
            }

            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
            }
        });
    }

    /**
     * Sets the image for the title-bar, so IF it shows in the task-bar, it will have the corresponding image as the SystemTray image
     */
    public
    void setTitleBarImage(final File imageFile) {
        if (this.iconFile == null || !this.iconFile.equals(imageFile)) {
            this.iconFile = imageFile;

            try {
                if (imageFile != null) {
                    Image image = new ImageIcon(ImageIO.read(imageFile)).getImage();
                    image.flush();

                    // we set the dialog window to have the same icon as what is on the system tray
                    hiddenDialog.setIconImage(image);
                }
            } catch (IOException e) {
                SystemTray.logger.error("Error setting the title-bar image for the popup menu task tray dialog");
            }
        }
    }

    void close() {
        hiddenDialog.setVisible(false);
        hiddenDialog.dispatchEvent(new WindowEvent(hiddenDialog, WindowEvent.WINDOW_CLOSING));
    }

    public
    void doShow(final Point point, int offset) {
        Dimension size = getPreferredSize();
        Rectangle bounds = ScreenUtil.getScreenBoundsAt(point);

        int x = point.x;
        int y = point.y;


        if (y < bounds.y) {
            y = bounds.y;
        }
        else if (y + size.height > bounds.y + bounds.height) {
            // our menu cannot have the top-edge snap to the mouse
            // so we make the bottom-edge snap to the mouse
            y -= size.height; // snap to edge of mouse
        }

        if (x < bounds.x) {
            x = bounds.x;
        }
        else if (x + size.width > bounds.x + bounds.width) {
            // our menu cannot have the left-edge snap to the mouse so we make the right-edge snap to the mouse
            x -= size.width; // snap right edge of menu to mouse

            offset = -offset; // flip offset
        }

        // display over the AppIndicator menu (which has to show, then we remove. THIS IS A HACK!)
        x -= offset;

        // System.err.println("show " + x + "," + y);

        // critical to get the keyboard listeners working for the popup menu
        setInvoker(hiddenDialog.getContentPane());


        hiddenDialog.setLocation(x, y);
        hiddenDialog.setVisible(true);

        setLocation(x, y);
        setVisible(true);

        requestFocusInWindow();
    }
}
