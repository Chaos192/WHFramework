// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePanel.h"
#include "Http/FileDownloader/FileDownloader.h"

class SDownloadsPanel;

struct FDownloadListItem : public TSharedFromThis<FDownloadListItem>
{
public:
	FDownloadListItem()
	{
		FileIcon = nullptr;
		FileDownloader = nullptr;
	}
	
	FDownloadListItem(const FSlateBrush* const FileIcon, TSharedPtr<FFileDownloader> FileDownloader)
		: FileIcon(FileIcon),
		  FileDownloader(FileDownloader)
	{
	}
	
public:
	const FSlateBrush* FileIcon;
	TSharedPtr<FFileDownloader> FileDownloader;
};

class SDownloadListItem : public STableRow<TSharedPtr<FDownloadListItem>>
{
public:
	SLATE_BEGIN_ARGS(SDownloadListItem) {}

		SLATE_ARGUMENT(FMargin, Padding)

		SLATE_ARGUMENT(TSharedPtr<FDownloadListItem>, DownloadListItem)
	
		SLATE_ARGUMENT(bool, IsRecentItem)

	SLATE_END_ARGS()

	SDownloadListItem();

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs, TSharedPtr<SBasePanel> InDownloadPanel, const TSharedRef<STableViewBase>& InOwnerTableView);

private:
	void HandleDownloadProgress(int64 BytesSent, int64 BytesReceived, int64 FullSize);

	void HandleDownloadComplete(EDownloadToStorageResult Result, const FString& SavePath);

	FReply OnClearButtonClicked();

private:
	TSharedPtr<SBasePanel> DownloadPanel;
	
	TSharedPtr<FDownloadListItem> DownloadListItem;

	TSharedPtr<STextBlock> Txt_DownloadProgress;

	TSharedPtr<STextBlock> Txt_DownloadRate;

	bool bRecentItem;
	
	int64 LastBytesReceived;

	float UpdateRemainTime;
};
