package com.galaev.audio;

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import static com.galaev.audio.AudioApiWrapper.*;

public class AudioFrame extends JFrame {
    private static final Dimension INITIAL_SIZE = new Dimension(300, 300);

    public AudioFrame(boolean initMute, int initVolume) {
        // window settings
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setTitle("Управление микшером");
        setMinimumSize(INITIAL_SIZE);
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        int screenWidth = (int) toolkit.getScreenSize().getWidth();
        int screenHeight = (int) toolkit.getScreenSize().getHeight();
        setLocation((screenWidth-getWidth())/2, (screenHeight-getHeight())/2);
        setLayout(new BorderLayout());

        // south button
        final JButton muteButton = new JButton();
        muteButton.setText(getMute() ? "Включить звук" : "Отключить звук");
        muteButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                boolean mute = ! getMute();
                JButton source = (JButton) e.getSource();
                source.setText(mute ? "Включить звук" : "Отключить звук");
                setMute(mute);
            }
        });
        add(muteButton, BorderLayout.SOUTH);

        // north label

        JLabel nameLabel = new JLabel("Галаев Антон 372(1)ПИ");
        nameLabel.setHorizontalAlignment(SwingConstants.CENTER);
        add(nameLabel, BorderLayout.NORTH);

        // west volume panel
        JPanel volumePanel = new JPanel();
        volumePanel.setLayout(new BoxLayout(volumePanel, BoxLayout.PAGE_AXIS));
        volumePanel.setAlignmentX(CENTER_ALIGNMENT);
        JSlider volumeSlider = new JSlider(SwingConstants.VERTICAL, 0 , 100, initVolume);
        volumeSlider.setMajorTickSpacing(50);
        volumeSlider.setMinorTickSpacing(5);
        volumeSlider.setPaintTicks(true);
        volumeSlider.setPaintLabels(true);
        volumeSlider.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                JSlider source = (JSlider) e.getSource();
                if (! source.getValueIsAdjusting()) {
                    int newValue = source.getValue();
                    setVolumeLevel(newValue / 100.0f);
                    if (getMute()) {
                        muteButton.doClick();
                    }
                }
            }
        });
        JLabel volumeLabel = new JLabel("Общий");
        volumeLabel.setAlignmentX(CENTER_ALIGNMENT);
        volumePanel.add(volumeSlider);
        volumePanel.add(volumeLabel);

        //  center bass volume panel
        JPanel bassPanel = new JPanel();
        bassPanel.setLayout(new BoxLayout(bassPanel, BoxLayout.PAGE_AXIS));
        bassPanel.setAlignmentX(CENTER_ALIGNMENT);
        bassSlider = new JSlider(SwingConstants.VERTICAL, 0 , 100, 50);
        bassSlider.setMajorTickSpacing(50);
        bassSlider.setMinorTickSpacing(5);
        bassSlider.setPaintTicks(true);
        bassSlider.setPaintLabels(true);
        bassSlider.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                JSlider source = (JSlider) e.getSource();
                if (!source.getValueIsAdjusting()) {
                    int newValue = source.getValue();
                    setBassLevel(newValue / 100.0f);
                }
            }
        });
        JLabel bassLabel = new JLabel("Нижние");
        bassLabel.setAlignmentX(CENTER_ALIGNMENT);
        bassPanel.add(bassSlider);
        bassPanel.add(bassLabel);

        //  center treble volume panel
        JPanel treblePanel = new JPanel();
        treblePanel.setLayout(new BoxLayout(treblePanel, BoxLayout.PAGE_AXIS));
        treblePanel.setAlignmentX(CENTER_ALIGNMENT);
        trebleSlider = new JSlider(SwingConstants.VERTICAL, 0 , 100, 50);
        trebleSlider.setMajorTickSpacing(50);
        trebleSlider.setMinorTickSpacing(5);
        trebleSlider.setPaintTicks(true);
        trebleSlider.setPaintLabels(true);
        trebleSlider.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent e) {
                JSlider source = (JSlider) e.getSource();
                if (!source.getValueIsAdjusting()) {
                    int newValue = source.getValue();
                    setTrebleLevel(newValue / 100.0f);
                }
            }
        });
        JLabel trebleLabel = new JLabel("Высокие");
        trebleLabel.setAlignmentX(CENTER_ALIGNMENT);
        treblePanel.add(trebleSlider);
        treblePanel.add(trebleLabel);

        JPanel mainPanel = new JPanel();
        mainPanel.add(volumePanel);
        mainPanel.add(bassPanel);
        mainPanel.add(treblePanel);
        add(mainPanel, BorderLayout.CENTER);
    }

    public void configureBass(float minBass, float maxBass, float currBass, float stepping) {
          if (stepping != -1.0f) {
              bassSlider = new JSlider(SwingConstants.VERTICAL, (int)minBass , (int)maxBass, (int)currBass);
              bassSlider.setMajorTickSpacing((int)(maxBass - minBass)/2);
              bassSlider.setMinorTickSpacing((int)stepping);
              bassSlider.setPaintTicks(true);
              bassSlider.setPaintLabels(true);
          } else {
              bassSlider.setEnabled(false);
              bassSlider.setToolTipText("Регулировка частот не поддерживается вашей звуковой платой");
              JOptionPane.showMessageDialog(null, "Регулировка частот не поддерживается вашей звуковой платой.",
                      "Предупреждение", JOptionPane.INFORMATION_MESSAGE);
          }
    }

    public void configureTreble(float minTreble, float maxTreble, float currTreble, float stepping) {
        if (stepping != -1.0f) {
            trebleSlider = new JSlider(SwingConstants.VERTICAL, (int)minTreble , (int)maxTreble, (int)currTreble);
            trebleSlider.setMajorTickSpacing((int)(maxTreble - minTreble)/2);
            trebleSlider.setMinorTickSpacing((int)stepping);
            trebleSlider.setPaintTicks(true);
            trebleSlider.setPaintLabels(true);
        } else {
            trebleSlider.setEnabled(false);
            trebleSlider.setToolTipText("Регулировка частот не поддерживается вашей звуковой платой");
        }
    }

    private JSlider bassSlider;
    private JSlider trebleSlider;

    public static void main(String[] args) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                } catch (Exception e) {
                    e.printStackTrace();
                }
                AudioFrame frame = new AudioFrame(getMute(), (int)(getVolumeLevel() * 100));
                frame.setVisible(true);
                frame.configureBass(getBassMin(), getBassMax(), getBassLevel(), getBassStepping());
                frame.configureTreble(getTrebleMin(), getTrebleMax(), getTrebleLevel(), getTrebleStepping());
            }
        });
    }
}
