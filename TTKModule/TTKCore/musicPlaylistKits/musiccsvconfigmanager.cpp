#include "musiccsvconfigmanager.h"

MusicCSVConfigManager::MusicCSVConfigManager()
    : MusicPlaylistReader()
    , MusicPlaylistInterface()
{

}

bool MusicCSVConfigManager::readPlaylistData(MusicSongItems &items)
{
    MusicSongItem item;
    item.m_itemName = QFileInfo(m_file.fileName()).baseName();

    QStringList data(QString(m_file.readAll()).split("\n"));
    if(data.isEmpty())
    {
        return false;
    }

    for(const QString &line : qAsConst(data))
    {
        const QStringList &songInfo = line.split(",");
        if(songInfo.count() > 2)
        {
            MusicSong song;
            song.setMusicName(songInfo[0]);
            song.setMusicPlayTime(songInfo[1]);
            song.setMusicPath(songInfo[2]);
            item.m_songs << song;
        }
    }
    m_file.close();

    if(!item.m_songs.isEmpty())
    {
        items << item;
    }
    return true;
}

bool MusicCSVConfigManager::writePlaylistData(const MusicSongItems &items, const QString &path)
{
    if(items.isEmpty())
    {
        return false;
    }

    const MusicSongItem &item = items.first();
    QStringList data;
    for(const MusicSong &song : qAsConst(item.m_songs))
    {
        data << song.getMusicName() + "," + song.getMusicPlayTime() + "," + song.getMusicPath();
    }

    m_file.setFileName(path);
    if(m_file.open(QFile::WriteOnly))
    {
        m_file.write(data.join("\n").toUtf8());
        m_file.close();
    }
    return true;
}
