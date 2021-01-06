#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define TIMER_PERFORMANCE_TESTING

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void tmrDataGenerationTimer_Tick();

#ifdef TIMER_PERFORMANCE_TESTING
    void tmrFrameCounter_Tick();
#endif

private:
    Ui::MainWindow *ui;
    void RegenerateXAxisData();
    void UpdateAxisData();

protected:
    virtual void keyPressEvent(QKeyEvent *KeyEventArgs);
    virtual void keyReleaseEvent(QKeyEvent *KeyEventArgs);
};

#endif // MAINWINDOW_H
